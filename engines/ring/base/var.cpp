/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "ring/base/var.h"

#include "ring/helpers.h"

namespace Ring {

#pragma region VarEntry

template<class T>
void VarEntry<T>::saveLoadWithSerializer(Common::Serializer &s) {
	// We need to provide a specific specialization for each type
	// We error out in case there isn't a valid one
	error("[VarEntry::saveLoadWithSerializer] Invalid specialization");
}

template<>
void VarEntry<byte>::saveLoadWithSerializer(Common::Serializer &s) {
	s.syncAsUint32LE(_id);
	s.syncAsByte(_value);
}

template<>
void VarEntry<int16>::saveLoadWithSerializer(Common::Serializer &s) {
	s.syncAsUint32LE(_id);
	s.syncAsSint16LE(_value);
}

template<>
void VarEntry<int32>::saveLoadWithSerializer(Common::Serializer &s) {
	s.syncAsUint32LE(_id);
	s.syncAsSint32LE(_value);
}

template<>
void VarEntry<double>::saveLoadWithSerializer(Common::Serializer &s) {
	s.syncAsUint32LE(_id);
	s.syncAsSint32LE(_value);
}

template<>
void VarEntry<Common::String>::saveLoadWithSerializer(Common::Serializer &s) {
	s.syncAsUint32LE(_id);
	s.syncString(_value);
}

#pragma endregion

#pragma region Var

Var::Var() {
}

Var::~Var() {
	removeAll();
}

void Var::removeAll() {
	CLEAR_ARRAY(VarEntry<byte>, _bytes);
	CLEAR_ARRAY(VarEntry<int16>, _words);
	CLEAR_ARRAY(VarEntry<int32>, _dwords);
	CLEAR_ARRAY(VarEntry<Common::String>, _strings);
	CLEAR_ARRAY(VarEntry<double>, _floats);
}

void Var::saveLoadWithSerializer(Common::Serializer &s) {
	_bytes.saveLoadWithSerializer(s);
	_words.saveLoadWithSerializer(s);
	_dwords.saveLoadWithSerializer(s);
	_strings.saveLoadWithSerializer(s);
	_floats.saveLoadWithSerializer(s);
}

template<typename T>
void Var::define(AssociativeArray<VarEntry<T> *> *array, Id id, T value) {
	if (array->has(id))
		error("[Var::define] ID already exists (%d)", id);

	array->push_back(new VarEntry<T>(id, value));
}

template<typename T>
T Var::get(AssociativeArray<VarEntry<T> *> *array, Id id) {
	if (!array->has(id))
		error("[Var::get] ID doesn't exists (%d)", id);

	return array->get(id)->get();
}

template<typename T>
void Var::set(AssociativeArray<VarEntry<T> *> *array, Id id, T value) {
	if (!array->has(id))
		error("[Var::set] ID doesn't exists (%d)", id);

	array->get(id)->set(value);
}

#pragma endregion

} // End of namespace Ring
