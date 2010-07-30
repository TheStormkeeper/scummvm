// -----------------------------------------------------------------------------
// This file is part of Broken Sword 2.5
// Copyright (c) Malte Thiesen, Daniel Queteschiner and Michael Elsd�rfer
//
// Broken Sword 2.5 is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Broken Sword 2.5 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Broken Sword 2.5; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
// -----------------------------------------------------------------------------

#ifndef SWORD25_BITMAP_RESOURCE_H
#define SWORD25_BITMAP_RESOURCE_H

// Includes
#include "sword25/kernel/common.h"
#include "sword25/kernel/resource.h"
#include "sword25/gfx/image/image.h"

// -----------------------------------------------------------------------------
// Forward Declarations
// -----------------------------------------------------------------------------

class BS_Rect;

class BS_BitmapResource : public BS_Resource
{
public:
	/**
		@brief Die m�glichen Flippingparameter f�r die Blit-Methode.
	*/
	enum FLIP_FLAGS
	{
		/// Das Bild wird nicht gespiegelt.
		FLIP_NONE = 0,
		/// Das Bild wird an der horizontalen Achse gespiegelt.
		FLIP_H = 1,
		/// Das Bild wird an der vertikalen Achse gespiegelt.
		FLIP_V = 2,
		/// Das Bild wird an der horizontalen und vertikalen Achse gespiegelt.
		FLIP_HV = FLIP_H | FLIP_V,
		/// Das Bild wird an der horizontalen und vertikalen Achse gespiegelt.
		FLIP_VH = FLIP_H | FLIP_V
	};

	BS_BitmapResource(const std::string & Filename, BS_Image * pImage);
	virtual ~BS_BitmapResource();

	/**
		@brief Gibt zur�ck, ob das Objekt einen g�ltigen Zustand hat.
	*/
	bool IsValid() const { return m_Valid; }

	/**
		@brief Gibt die Breite des Bitmaps zur�ck.
	*/
	int GetWidth() const { BS_ASSERT(m_pImage); return m_pImage->GetWidth(); }

	/**
		@brief Gibt die H�he des Bitmaps zur�ck.
	*/
	int GetHeight() const { BS_ASSERT(m_pImage); return m_pImage->GetHeight(); }

	/**
		@brief Rendert das Bild in den Framebuffer.
		@param PosX die Position auf der X-Achse im Zielbild in Pixeln, an der das Bild gerendert werden soll.<br>
					Der Standardwert ist 0.
		@param PosY die Position auf der Y-Achse im Zielbild in Pixeln, an der das Bild gerendert werden soll.<br>
					Der Standardwert ist 0.
		@param Flipping gibt an, wie das Bild gespiegelt werden soll.<br>
						Der Standardwert ist BS_Image::FLIP_NONE (keine Spiegelung)
		@param pSrcPartRect Pointer auf ein BS_Rect, welches den Ausschnitt des Quellbildes spezifiziert, der gerendert
							werden soll oder NULL, falls das gesamte Bild gerendert werden soll.<br>
							Dieser Ausschnitt bezieht sich auf das ungespiegelte und unskalierte Bild.<br>
							Der Standardwert ist NULL.
		@param Color ein ARGB Farbwert, der die Parameter f�r die Farbmodulation und f�rs Alphablending festlegt.<br>
					 Die Alpha-Komponente der Farbe bestimmt den Alphablending Parameter (0 = keine Deckung, 255 = volle Deckung).<br>
					 Die Farbkomponenten geben die Farbe f�r die Farbmodulation an.<br>
					 Der Standardwert is BS_ARGB(255, 255, 255, 255) (volle Deckung, keine Farbmodulation).
					 Zum Erzeugen des Farbwertes k�nnen die Makros BS_RGB und BS_ARGB benutzt werden.
		@param Width gibt die Ausgabebreite des Bildausschnittes an. 
					 Falls diese von der Breite des Bildausschnittes abweicht wird
					 das Bild entsprechend Skaliert.<br>
					 Der Wert -1 gibt an, dass das Bild nicht Skaliert werden soll.<br>
					 Der Standardwert ist -1.
		@param Width gibt die Ausgabeh�he des Bildausschnittes an. 
					 Falls diese von der H�he des Bildauschnittes abweicht, wird
					 das Bild entsprechend Skaliert.<br>
					 Der Wert -1 gibt an, dass das Bild nicht Skaliert werden soll.<br>
					 Der Standardwert ist -1.
		@return Gibt false zur�ck, falls das Rendern fehlgeschlagen ist.
		@remark Er werden nicht alle Blitting-Operationen von allen BS_Image-Klassen unterst�tzt.<br>
				Mehr Informationen gibt es in der Klassenbeschreibung von BS_Image und durch folgende Methoden:
				- IsBlitTarget()
				- IsScalingAllowed()
				- IsFillingAllowed()
				- IsAlphaAllowed()
				- IsColorModulationAllowed()
	*/
	bool Blit(int PosX = 0, int PosY = 0, 
			  int Flipping = FLIP_NONE, 
			  BS_Rect* pSrcPartRect = NULL,
			  unsigned int Color = BS_ARGB(255, 255, 255, 255),
			  int Width = -1, int Height = -1)
	{
		BS_ASSERT(m_pImage);
		return m_pImage->Blit(PosX, PosY, Flipping, pSrcPartRect, Color, Width, Height);
	}

	/**
		@brief F�llt einen Rechteckigen Bereich des Bildes mit einer Farbe.
		@param pFillRect Pointer auf ein BS_Rect, welches den Ausschnitt des Bildes spezifiziert, der gef�llt
						  werden soll oder NULL, falls das gesamte Bild gef�llt werden soll.<br>
						  Der Standardwert ist NULL.
		@param Color der 32 Bit Farbwert mit dem der Bildbereich gef�llt werden soll.
		@remark Ein Aufruf dieser Methode ist nur gestattet, wenn IsFillingAllowed() true zur�ckgibt.
		@remark Es ist m�glich �ber die Methode transparente Rechtecke darzustellen, indem man eine Farbe mit einem Alphawert ungleich
				255 angibt.
		@remark Unabh�ngig vom Farbformat des Bildes muss ein 32 Bit Farbwert angegeben werden. Zur Erzeugung, k�nnen die Makros
				BS_RGB und BS_ARGB benutzt werden.
		@remark Falls das Rechteck nicht v�llig innerhalb des Bildschirms ist, wird es automatisch zurechtgestutzt.
	*/
	bool Fill(const BS_Rect* pFillRect = 0, unsigned int Color = BS_RGB(0, 0, 0)) { BS_ASSERT(m_pImage); return m_pImage->Fill(pFillRect, Color); }

	/**
		@brief Liest einen Pixel des Bildes.
		@param X die X-Koordinate des Pixels.
		@param Y die Y-Koordinate des Pixels
		@return Gibt den 32-Bit Farbwert des Pixels an der �bergebenen Koordinate zur�ck.
		@remark Diese Methode sollte auf keine Fall benutzt werden um gr��ere Teile des Bildes zu lesen, da sie sehr langsam ist. Sie ist
				eher daf�r gedacht einzelne Pixel des Bildes auszulesen.
	*/
	unsigned int GetPixel(int X, int Y) const;

	//@{
	/** @name Auskunfts-Methoden */

	/**
		@brief �berpr�ft, ob das BS_Image ein Zielbild f�r einen Blit-Aufruf sein kann.
		@return Gibt false zur�ck, falls ein Blit-Aufruf mit diesem Objekt als Ziel nicht gestattet ist.
	*/
	bool IsBlitTarget() { BS_ASSERT(m_pImage); return m_pImage->IsBlitTarget(); }

	/**
		@brief Gibt true zur�ck, falls das BS_Image bei einem Aufruf von Blit() skaliert dargestellt werden kann.
	*/
	bool IsScalingAllowed() { BS_ASSERT(m_pImage); return m_pImage->IsScalingAllowed(); }

	/**
		@brief Gibt true zur�ck, wenn das BS_Image mit einem Aufruf von Fill() gef�llt werden kann.
	*/
	bool IsFillingAllowed() { BS_ASSERT(m_pImage); return m_pImage->IsFillingAllowed(); }

	/**
		@brief Gibt true zur�ck, wenn das BS_Image bei einem Aufruf von Blit() mit einem Alphawert dargestellt werden kann.
	*/
	bool IsAlphaAllowed() { BS_ASSERT(m_pImage); return m_pImage->IsAlphaAllowed(); }

	/**
		@brief Gibt true zur�ck, wenn das BS_Image bei einem Aufruf von Blit() mit Farbmodulation dargestellt werden kann.
	*/
	bool IsColorModulationAllowed() { BS_ASSERT(m_pImage); return m_pImage->IsColorModulationAllowed(); }

private:
	BS_Image *	m_pImage;
	bool		m_Valid;
};

#endif
