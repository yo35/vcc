/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2014 Yoann Le Montagner <yo35(at)melix(dot)net>      *
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


#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <memory>


/**
 * Singleton design pattern.
 *
 * To make a class `Foo` become a singleton, define it as follows:
 *
 *     class Foo : public Singleton<Foo>
 *     {
 *         friend class Singleton<Foo>;
 *
 *         ...
 *
 *     private:
 *        Foo();
 *     };
 *
 * Singleton classes such as `Foo` should not be inherited. That is the reason
 * why the constructor of `Foo` is declared as private, and not as protected.
 */
template<typename T>
class Singleton
{
public:

	/**
	 * Return the unique instance of the class `T`.
	 */
	static T &instance()
	{
		if(!_instance) {
			_instance.reset(new T);
		}
		return *_instance;
	}

protected:

	/**
	 * The default constructor is protected: instances of the template class must not
	 * be created outside the constructors of derived classes.
	 */
	Singleton() {}

private:

	/**
	 * Pointer to the singleton object.
	 */
	static std::unique_ptr<T> _instance;
};


/**
 * Singleton object.
 */
template<typename T>
std::unique_ptr<T> Singleton<T>::_instance;


#endif /* SINGLETON_H_ */
