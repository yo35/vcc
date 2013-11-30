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


#ifndef ENUMUTIL_H_
#define ENUMUTIL_H_

#include <cstddef>
#include <cassert>
#include <type_traits>
#include <initializer_list>
#include <ostream>
#include <istream>


/**
 * Utility classes and functions to deal with enums.
 */
namespace Enum
{
	/**
	 * Trait template class related to enum features.
	 */
	template<typename E>
	struct traits
	{
		static constexpr bool does_index = false;
	};


	/**
	 * Trait class indicating that an enum supports indexing.
	 *
	 * @tparam N Number of elements in the corresponding enum.
	 */
	template<std::size_t N>
	struct trait_indexing
	{
		static constexpr bool does_index = true;
		static constexpr std::size_t count = N;
	};


	/**
	 * Return the index value of an enum constant.
	 *
	 * @tparam E Enum class, supposed to support indexing.
	 */
	template<typename E>
	constexpr inline
	typename std::enable_if<traits<E>::does_index, typename std::underlying_type<E>::type>::type
	to_value(E e) noexcept
	{
		return static_cast<typename std::underlying_type<E>::type>(e);
	}


	/**
	 * Interpret an integral value as an index refering to an enum constant.
	 *
	 * @tparam E Enum class, supposed to support indexing.
	 */
	template<typename E, typename T>
	constexpr inline
	typename std::enable_if<traits<E>::does_index && std::is_integral<T>::value, E>::type
	from_value(T value)
	{
		//assert(static_cast<std::size_t>(value) < traits<E>::count);
		return static_cast<E>(value);
	}



	template<typename E, typename enabled=void>
	class cursor;

	template<typename E, typename T, typename enabled=void>
	class array;



	/**
	 * Cursor to visit the constants of an enum class.
	 *
	 * @tparam E Enum class, supposed to support indexing.
	 */
	template<typename E>
	class cursor<E, typename std::enable_if<traits<E>::does_index>::type>
	{
	public:

		/**
		 * Default constructor.
		 */
		cursor() : _index(0) {}

		/**
		 * Construct a cursor pointing the enum constant `e`.
		 */
		explicit cursor(E e) : _index(to_value(e)) {}

		/**
		 * Return a cursor pointing to the first constant of the corresponding enum class.
		 */
		static cursor first() { return cursor(0); }

		/**
		 * Return a cursor pointing to the last constant of the corresponding enum class.
		 */
		static cursor last () { return cursor(static_cast<int>(traits<E>::count) - 1); }

		/**
		 * Whether the cursor points to a valid enum constant or not.
		 */
		bool valid() const { return _index>=0 && _index<static_cast<int>(traits<E>::count); }

		/**
		 * Dereference operator: return the underlying enum constant.
		 */
		E operator*() const { return from_value<E>(_index); }

		/**
		 * @name Comparison operators.
		 * @{
		 */
		bool operator==(const cursor &op) const { return _index==op._index; }
		bool operator!=(const cursor &op) const { return _index!=op._index; }
		bool operator<=(const cursor &op) const { return _index<=op._index; }
		bool operator< (const cursor &op) const { return _index< op._index; }
		bool operator> (const cursor &op) const { return _index> op._index; }
		bool operator>=(const cursor &op) const { return _index>=op._index; }
		/**@} */

		/**
		 * @name Displacement operators.
		 * @{
		 */
		cursor &operator++()        { ++_index;    return *this; }
		cursor &operator--()        { --_index;    return *this; }
		cursor &operator+=(int val) { _index+=val; return *this; }
		cursor &operator-=(int val) { _index-=val; return *this; }
		/**@} */

	private:

		// Private constructor
		explicit cursor(int index) : _index(index) {}

		// Private members
		int _index;
	};



	/**
	 * Array indexed by the constants of an enum class.
	 *
	 * @tparam E Enum class, supposed to support indexing.
	 * @tparam T Type of the elements of the array.
	 */
	template<typename E, typename T>
	class array<E, T, typename std::enable_if<traits<E>::does_index>::type>
	{
	public:

		/**
		 * Default constructor.
		 */
		array() = default;

		/**
		 * Construction from an initializer list. The initialization list must have a length
		 * equal to the number of constants of the enum class.
		 */
		array(std::initializer_list<T> values)
		{
			assert(values.size()==traits<E>::count);
			std::size_t e=0;
			for(auto it=values.begin(); it!=values.end(); ++it, ++e) {
				_data[e] = *it;
			}
		}

		/**
		 * Access to an element of the array.
		 * @param e Index.
		 */
		T &operator[](E e) { return _data[to_value(e)]; }

		/**
		 * Access to an element of the array (constant).
		 * @param e Index.
		 */
		const T &operator[](E e) const { return _data[to_value(e)]; }

	private:

		// Private members
		T _data[traits<E>::count];
	};

} // namespace Enum


/**
 * Output stream operator for enum values.
 */
template<typename E>
typename std::enable_if<Enum::traits<E>::does_index, std::ostream &>::type
operator<<(std::ostream &stream, E e)
{
	return stream << static_cast<int>(Enum::to_value(e));
}


/**
 * Input stream operator for enum values.
 */
template<typename E>
typename std::enable_if<Enum::traits<E>::does_index, std::istream &>::type
operator>>(std::istream &stream, E &e)
{
	int buffer = 0;
	stream >> buffer;
	if(buffer>=0 && buffer<static_cast<int>(Enum::traits<E>::count)) {
		e = Enum::from_value<E>(buffer);
	}
	else {
		stream.setstate(std::ios_base::failbit);
	}
	return stream;
}


#endif /* ENUMUTIL_H_ */
