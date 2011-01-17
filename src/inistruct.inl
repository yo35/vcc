/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2011  Yoann Le Montagner <yo35(at)melix(dot)net>     *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                            *
 ******************************************************************************/


// Lecture d'un type énuméré
template<class T>
Enumerable<T> IniStruct::get_data(const Key &section, const Key &code,
	const Enumerable<T> &default_value) const
{
	int retval = get_data(section, code, default_value.to_int());
	if(retval>=0 && retval<Enumerable<T>::BaseType::N)
		return Enumerable<T>(retval);
	else
		return default_value;
}

// Écriture d'un type énuméré
template<class T>
void IniStruct::set_data(const Key &section, const Key &code, const Enumerable<T> &value) {
	set_data(section, code, value.to_int());
}
