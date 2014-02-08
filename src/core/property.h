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


#ifndef PROPERTY_H_
#define PROPERTY_H_

#include <type_traits>
#include <functional>
#include <wrappers/signals.h>


/**
 * Base class for properties, that provides the methods that are independent
 * of the underlying property type.
 */
class AbstractProperty
{
public:

	/**
	 * Destructor.
	 */
	virtual ~AbstractProperty() {}

	/**
	 * @name Neither copying nor moving a property is allowed.
	 * @{
	 */
	AbstractProperty(const AbstractProperty &op) = delete;
	AbstractProperty(AbstractProperty &&op) = delete;
	AbstractProperty &operator=(const AbstractProperty &op) = delete;
	AbstractProperty &operator=(AbstractProperty &&op) = delete;
	/**@} */

	/**
	 * Whether the property has already been loaded or not.
	 */
	bool loaded() const { return _loaded; }

	/**
	 * Load the property, if not done yet. Return `true` if the property is actually loaded by the call.
	 */
	virtual bool load() const=0;

protected:

	/**
	 * Constructor.
	 */
	AbstractProperty() : _loaded(false) {}

	/**
	 * Whether the property has already been loaded or not.
	 */
	bool _loaded;
};



/**
 * Base class for read-only properties.
 */
class AbstractReadOnlyProperty : public AbstractProperty
{
protected:

	/**
	 * Constructor.
	 */
	AbstractReadOnlyProperty() {}
};



/**
 * Base class for writable properties.
 */
class AbstractReadWriteProperty : public AbstractProperty
{
public:

	/**
	 * Whether the saved (i.e. persistent) state of the property reflects its current (i.e. logical) state.
	 */
	bool saved() const { return _saved; }

	/**
	 * Save the property, if necessary. Return `true` if the property is actually saved by the call.
	 */
	virtual bool save()=0;

	/**
	 * Signal triggered when the value of the property is saved.
	 */
	sig::connection connect_saved(const sig::signal<void()>::slot_type &slot) const
	{
		return _signal_saved.connect(slot);
	}

protected:

	/**
	 * Constructor.
	 */
	AbstractReadWriteProperty() : _saved(true) {}

	/**
	 * Whether the saved (i.e. persistent) state of the property reflects its current (i.e. logical) state.
	 */
	bool _saved;

	/**
	 * Signal triggered when the value of the property is saved.
	 */
	mutable sig::signal<void()> _signal_saved;
};



/**
 * Trait class used to define the property classes.
 */
template<typename T>
struct PropertyTraits
{
	/**
	 * Type of the underlying value.
	 */
	typedef typename std::remove_reference<T>::type type_t;

	/**
	 * Either the type of the underlying value if is a scalar type, or a constant reference to it.
	 */
	typedef typename std::conditional<std::is_scalar<type_t>::value, type_t, const type_t &>::type lightweight_type_t;

	/**
	 * Type of the loader (this is a function).
	 */
	typedef std::function<void(type_t &)> loader;

	/**
	 * Type of the saver (this is a function).
	 */
	typedef std::function<void(lightweight_type_t)> saver;

	/**
	 * Type of the signal indicating a modification of the underlying value.
	 */
	typedef sig::signal<void(lightweight_type_t)> change_signal;

	/**
	 * Type of the signal indicating a modification of the underlying value.
	 */
	typedef typename sig::signal<void(lightweight_type_t)>::slot_type change_slot;
};



/**
 * Read-only property.
 */
template<typename T, typename traits=PropertyTraits<T>>
class ReadOnlyProperty : public AbstractReadOnlyProperty
{
public:

	/**
	 * Constructor.
	 */
	ReadOnlyProperty(typename traits::loader loader) : _loader(loader) {}

	/**
	 * Read the underlying value. The loader is automatically called if not done yet.
	 */
	typename traits::lightweight_type_t operator()() const
	{
		const_cast<ReadOnlyProperty *>(this)->ensure_loaded(); return _data;
	}

	// Implement the load method.
	bool load() const override
	{ bool retval=!_loaded; const_cast<ReadOnlyProperty *>(this)->ensure_loaded(); return retval; }

private:

	// Initialize the object, if not done yet.
	void ensure_loaded()
	{
		if(_loaded) return;
		_loader(_data);
		_loaded = true;
	}

	// Private members
	typename traits::type_t _data  ;
	typename traits::loader _loader;
};



/**
 * Read-write property.
 */
template<typename T, typename traits=PropertyTraits<T>>
class ReadWriteProperty : public AbstractReadWriteProperty
{
public:

	/**
	 * Constructor.
	 */
	ReadWriteProperty(typename traits::loader loader, typename traits::saver saver) :
		_loader(loader), _saver(saver) {}

	/**
	 * Read the underlying value. The loader is automatically called if not done yet.
	 */
	typename traits::lightweight_type_t operator()() const
	{
		const_cast<ReadWriteProperty *>(this)->ensure_loaded(); return _data;
	}

	/**
	 * Write the underlying value.
	 */
	void operator()(typename traits::lightweight_type_t value)
	{
		_data=value; _loaded=true; _saved=false; _signal_changed(_data);
	}

	/**
	 * Signal triggered when the value of the property changes.
	 */
	sig::connection connect_changed(const typename traits::change_slot &slot) const
	{
		return _signal_changed.connect(slot);
	}

	// Implement the load method.
	bool load() const override
	{ bool retval=!_loaded; const_cast<ReadWriteProperty *>(this)->ensure_loaded(); return retval; }

	// Implement the save method.
	bool save() override { bool retval=!_saved; ensure_saved(); return retval; }

private:

	// Initialize the object, if not done yet.
	void ensure_loaded()
	{
		if(_loaded) return;
		_loader(_data);
		_loaded = true;
	}

	// Save the object, if not done yet.
	void ensure_saved()
	{
		if(_saved) return;
		_saver(_data);
		_saved = true;
		_signal_saved();
	}

	// Private members
	mutable typename traits::change_signal _signal_changed;
	typename traits::type_t _data  ;
	typename traits::loader _loader;
	typename traits::saver  _saver ;
};


#endif /* PROPERTY_H_ */
