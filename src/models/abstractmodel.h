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


#ifndef ABSTRACTMODEL_H_
#define ABSTRACTMODEL_H_

#include <type_traits>
#include <functional>
#include <set>
#include <wrappers/signals.h>


/**
 * Base class for models.
 */
class AbstractModel
{
public:

	/**
	 * Base class for the property of a model. This class provides the methods that are independent
	 * of the underlying type of the property.
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
		 * Whether the saved (i.e. persistent) state of the property reflects its current (i.e. logical) state.
		 */
		bool saved() const { return _saved; }

		/**
		 * Signal triggered when the value of the property changes.
		 */
		sig::connection connect_changed(const sig::signal<void()>::slot_type &slot)
		{
			return _signal_changed.connect(slot);
		}

		/**
		 * The base model class must be granted priviledged access to some methods
		 * of the `AbstractProperty` class.
		 */
		class Attorney
		{
			friend class AbstractModel;
			static bool load(const AbstractProperty &property) { return property.load(); }
			static bool save(AbstractProperty &property) { return property.save(); }
		};

	protected:

		/**
		 * Constructor.
		 */
		AbstractProperty() : _loaded(false), _saved(true) {}

		/**
		 * Load the property, if not done yet. Return `true` if the property is actually loaded by the call.
		 */
		virtual bool load() const=0;

		/**
		 * Save the property, if necessary. Return `true` if the property is actually saved by the call.
		 */
		virtual bool save()=0;

		/**
		 * Whether the property has already been loaded or not.
		 */
		bool _loaded;

		/**
		 * Whether the saved (i.e. persistent) state of the property reflects its current (i.e. logical) state.
		 */
		bool _saved;

		/**
		 * Signal triggered when the value of the property changes.
		 */
		sig::signal<void()> _signal_changed;
	};



	/**
	 * Property of a model. It holds a piece of data (of type `T`).
	 *
	 * This template class implements a lazy-loading strategy: the underlying value of the property
	 * is initialized only the first time it is accessed. This initialization is delegated to
	 * a loader function, that is provided in the constructor.
	 */
	template<typename T>
	class Property : public AbstractProperty
	{
	public:

		/**
		 * Type of the underlying value.
		 */
		typedef T type_t;

		/**
		 * Either the type of the underlying value if is a scalar type, or a constant reference to it.
		 */
		typedef typename std::conditional<std::is_scalar<T>::value, T, const T &>::type lightweight_type_t;

		/**
		 * Type of the loader (this is a function).
		 */
		typedef std::function<void(T &)> Loader;

		/**
		 * Type of the saver (this is a function).
		 */
		typedef std::function<void(lightweight_type_t)> Saver;

		/**
		 * Read the underlying value. The loader is automatically called if not done yet.
		 */
		lightweight_type_t operator()() const { const_cast<Property *>(this)->ensure_loaded(); return _data; }

		/**
		 * Write the underlying value.
		 */
		void operator()(lightweight_type_t value) { _data=value; _loaded=true; _saved=false; _signal_changed(); }

		/**
		 * Constructor for a read-only property. As no saver is provided, any subsequent call to the method `save`
		 * will result in a `std::bad_function_call` exception be thrown.
		 */
		Property(Loader loader) : _loader(loader) {}

		/**
		 * Constructor.
		 * @param loader This function will be called the first time the underlying value is read.
		 *        It is in charge of initializing this value.
		 * @param saver This function is called when the property is required to be saved.
		 */
		Property(Loader loader, Saver saver) : _loader(loader), _saver(saver) {}

	protected:

		// Implement the load method.
		bool load() const override { bool retval=!_loaded; const_cast<Property *>(this)->ensure_loaded(); return retval; }

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

		// Save the object, if necessary.
		void ensure_saved()
		{
			if(_saved) return;
			_saver(_data);
			_saved = true;
		}

		// Private members
		Loader _loader;
		Saver  _saver ;
		T      _data  ;
	};



	/**
	 * Destructor.
	 */
	virtual ~AbstractModel() {}

	/**
	 * @name Neither copy nor move is allowed.
	 * @{
	 */
	AbstractModel(const AbstractModel &op) = delete;
	AbstractModel(AbstractModel &&op) = delete;
	AbstractModel &operator=(const AbstractModel &op) = delete;
	AbstractModel &operator=(AbstractModel &&op) = delete;
	/**@} */

	/**
	 * Save the model.
	 */
	void save();

protected:

	/**
	 * Constructor.
	 */
	AbstractModel() {}

	/**
	 * Register the given constant property.
	 */
	void register_property(const AbstractProperty &property);

	/**
	 * Register the given writable property.
	 */
	void register_property(AbstractProperty &property);

	/**
	 * Called at the end of the `save()` method. Nothing done by default.
	 */
	virtual void finalize_save() {}

private:

	// Private members
	std::set<AbstractProperty *> _properties;
};

#endif /* ABSTRACTMODEL_H_ */
