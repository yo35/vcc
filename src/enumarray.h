/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2012 Yoann Le Montagner <yo35(at)melix(dot)net>      *
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


#ifndef ENUMARRAY_H_
#define ENUMARRAY_H_

#include "enumerable.h"
#include <cassert>

/**
 * Array indexed by an enumerable class
 * \param T Indexing class (should be \p Enumerable<?>)
 * \param V Type of the objects contained in the array
 */
template<class T, class V>
class EnumArray
{
public:

	/**
	 * Getter
	 * \pre idx<T::N
	 * \pre m_data!=nullptr
	 */
	V &operator[](T idx)
	{
		assert(m_data!=nullptr);
		assert(idx.to_numeric()<T::N);
		return m_data[idx.to_numeric()];
	}

	/**
	 * Constant getter
	 * \pre idx<T::N
	 * \pre m_data!=nullptr
	 */
	const V &operator[](T idx) const
	{
		assert(m_data!=nullptr);
		assert(idx.to_numeric()<T::N);
		return m_data[idx.to_numeric()];
	}

	/**
	 * Move operator
	 */
	EnumArray &operator=(EnumArray &&rhs)
	{
		if(this!=&rhs) {
			release_memory();
			m_data = rhs.m_data;
			rhs.m_data = nullptr;
		}
		return *this;
	}

	/**
	 * Copy operator
	 */
	EnumArray &operator=(const EnumArray &rhs)
	{
		if(this!=&rhs) {
			release_memory();
			if(rhs.m_data!=nullptr) {
				m_data = new V[T::N];
				for(std::size_t k=0; k<T::N; ++k) {
					m_data[k] = rhs.m_data[k];
				}
			}
		}
		return *this;
	}

	/**
	 * Move constructor
	 */
	EnumArray(EnumArray &&rhs) : m_data(rhs.m_data)
	{
		rhs.m_data = nullptr;
	}

	/**
	 * Copy constructor
	 */
	EnumArray(const EnumArray &rhs) : m_data(nullptr)
	{
		if(rhs.m_data!=nullptr) {
			m_data = new V[T::N];
			for(std::size_t k=0; k<T::N; ++k) {
				m_data[k] = rhs.m_data[k];
			}
		}
	}

	/**
	 * Default constructor
	 */
	EnumArray() : m_data(new V[T::N]) {}

	/**
	 * Destructor
	 */
	~EnumArray()
	{
		release_memory();
	}

private:

	// Release the allocated array if exists
	void release_memory()
	{
		if(m_data!=nullptr) {
			delete[] m_data;
			m_data = nullptr;
		}
	}

	// Private data
	V *m_data;
};

#endif /* ENUMARRAY_H_ */
