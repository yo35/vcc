/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>          *
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


#ifndef ENUMERABLE_H
#define ENUMERABLE_H

#include <cassert>

// Ensemble énumérable
template<class T>
class Enumerable {

public:

	// Alias vers le type de base
	typedef T BaseType;

	// Itérateurs
	class iterator {
		friend class Enumerable;

	public:

		// Construction et déréférencement
		iterator()                         { m_it=0; }
		const Enumerable operator*() const { return Enumerable<T>(m_it); }
		bool valid() const                 { return m_it>=0 && m_it<T::N; }

		// Déplacement
		iterator &operator++()        { ++m_it;    return *this; }
		iterator &operator--()        { --m_it;    return *this; }
		iterator &operator+=(int val) { m_it+=val; return *this; }
		iterator &operator-=(int val) { m_it-=val; return *this; }

		// Test
		bool operator==(const iterator &op) const { return m_it==op.m_it; }
		bool operator!=(const iterator &op) const { return m_it!=op.m_it; }

	private:
		explicit iterator(int src) { m_it=src; }
		int m_it;
	};

	// Constructeurs, comparaison
	Enumerable()                  { m_data=0; }
	explicit Enumerable(int src)  { assert(src>=0 && src<T::N); m_data=src; }
	int to_int() const            { return m_data; }

	// Itérateurs
	static const iterator first() { return iterator(0); }
	static const iterator last () { return iterator(T::N-1); }

	// Comparaison
	bool operator==(const Enumerable &src) const { return m_data==src.m_data; }
	bool operator!=(const Enumerable &src) const { return m_data!=src.m_data; }
	bool operator<=(const Enumerable &src) const { return m_data<=src.m_data; }
	bool operator< (const Enumerable &src) const { return m_data< src.m_data; }
	bool operator>=(const Enumerable &src) const { return m_data>=src.m_data; }
	bool operator> (const Enumerable &src) const { return m_data> src.m_data; }

private:

	// Données membres
	unsigned char m_data;
};



// Tableau indexé par un type énumérable
template<class T, class V>
class EnumArray {

public:

	// Accesseurs
	V &operator[](const T &idx) {
		assert(idx.to_int()>=0 && idx.to_int()<T::BaseType::N);
		return m_data[idx.to_int()];
	}
	const V &operator[](const T &idx) const {
		assert(idx.to_int()>=0 && idx.to_int()<T::BaseType::N);
		return m_data[idx.to_int()];
	}

private:

	// Données membres
	V m_data[T::BaseType::N];
};

#endif
