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


#ifndef ENUMERABLE_H_
#define ENUMERABLE_H_

#include <cassert>
#include <cstddef>

/**
 * Class with a finite set of values (can be thought as an enumeration)
 * \param T Specify the number of possible values in the class
 * \param U Base type used to encode the data
 */
template<class T, class U=unsigned char>
class Enumerable
{
public:

	/**
	 * Number of different values for the objects of the current Enumerable class
	 */
	static const std::size_t N = T::N;



	/**
	 * Object used to visit all the possible values of the current Enumerable
	 */
	class cursor
	{
		friend class Enumerable;
	public:

		/**
		 * Default constructor
		 */
		cursor() { m_it=0; }

		/**
		 * Current visited value
		 * \pre valid()
		 */
		const Enumerable operator*() const { return Enumerable(m_it); }

		/**
		 * Check if the cursor points to an allowed value in the underlying Enumerable class
		 */
		bool valid() const { return m_it>=0 && static_cast<std::size_t>(m_it)<N; }

		/**
		 * \name Cursor displacement
		 * @{
		 */
		cursor &operator++()        { ++m_it;    return *this; } //!< Next value
		cursor &operator--()        { --m_it;    return *this; } //!< Previous value
		cursor &operator+=(int val) { m_it+=val; return *this; } //!< Skipping \p val values forward
		cursor &operator-=(int val) { m_it-=val; return *this; } //!< Skipping \p val values backward
		///@}

		/**
		 * \name Comparison operators
		 * @{
		 */
		bool operator==(const cursor &op) const { return m_it==op.m_it; }
		bool operator!=(const cursor &op) const { return m_it!=op.m_it; }
		///@}

	private:
		explicit cursor(int val) { m_it=val; }
		int m_it;
	};



	/**
	 * Default constructor
	 */
	Enumerable() { m_data=0; }

	/**
	 * Create an object from its rank \p val
	 * \pre val<N
	 */
	explicit Enumerable(std::size_t val) { assert(val<N); m_data=val; }

	/**
	 * Extract the rank of the current object
	 */
	std::size_t to_numeric() const { return m_data; }

	/**
	 * \name Cursors
	 * @{
	 */
	static const cursor first() { return cursor(0  ); } //!< First value in the class
	static const cursor last () { return cursor(N-1); } //!< Last value in the class
	///@}

	/**
	 * \name Comparison operators
	 * @{
	 */
	bool operator==(const Enumerable &op) const { return m_data==op.m_data; }
	bool operator!=(const Enumerable &op) const { return m_data!=op.m_data; }
	bool operator<=(const Enumerable &op) const { return m_data<=op.m_data; }
	bool operator< (const Enumerable &op) const { return m_data< op.m_data; }
	bool operator>=(const Enumerable &op) const { return m_data>=op.m_data; }
	bool operator> (const Enumerable &op) const { return m_data> op.m_data; }
	///@}

private:

	// Private data
	U m_data;
};

#endif /* ENUMERABLE_H_ */
