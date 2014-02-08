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


#include "params.h"
#include <models/modelpaths.h>
#include <models/modelappinfo.h>
#include <models/modelkeyboard.h>
#include <config.h>
#include <stdexcept>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <QApplication>


// Singleton object.
std::unique_ptr<Params> Params::_instance;


// File that holds the preferences of the current user.
const std::string &Params::config_file()
{
	if(!_config_file) {
		_config_file = ModelPaths::instance().config_path() + "/" + ModelAppInfo::instance().short_name() + ".xml";
	}
	return *_config_file;
}


// Private constructor.
Params::Params() :
	_root(nullptr), _ptree_loaded(false), _ptree_saved(true)
{}


// Load the persistent file that holds the preferences defined by the current user,
// if it is not loaded yet.
void Params::load()
{
	// Nothing to do if the file has already been loaded.
	if(_ptree_loaded) {
		return;
	}

	// Load the file if it exists.
	if(boost::filesystem::exists(config_file())) {
		try {
			boost::property_tree::read_xml(config_file(), _ptree, boost::property_tree::xml_parser::trim_whitespace);
		}
		catch(boost::property_tree::xml_parser_error &) {
			throw std::runtime_error("An error has occurred while reading the preference file.");
		}
	}

	// Set up the root node.
	if(_ptree.find("options")==_ptree.not_found()) {
		_ptree.put_child("options", ptree());
	}
	_root = &_ptree.get_child("options");

	// Set the loaded flag to true.
	_ptree_loaded = true;
}


// Save the preferences defined by the current user.
void Params::save()
{
	// Nothing else to do if the file has already been saved.
	if(_ptree_saved) {
		return;
	}

	// Write the file
	ensure_config_path_exists();
	try {
		boost::property_tree::xml_writer_settings<ptree::key_type::value_type> settings('\t', 1);
		boost::property_tree::write_xml(config_file(), _ptree, std::locale(), settings);
	}
	catch(boost::property_tree::xml_parser_error &) {
		throw std::runtime_error("An error has occurred while writing the preference file.");
	}

	// Set the saved flag to true
	_ptree_saved = true;
}


// Create the application folder is the user's home if it does not exist yet.
void Params::ensure_config_path_exists()
{
	ModelPaths::instance().ensure_config_path_exists();
}


// Return the node corresponding to the given key (the node is created it if it does not exist yet).
Params::ptree &Params::fetch(const std::string &key)
{
	if(_root->find(key)==_root->not_found()) {
		_root->put_child(key, ptree());
	}
	return _root->get_child(key);
}


// Return an atomic-valued option.
template<class T>
T Params::get_atomic_value(const std::string &key, T default_value)
{
	load();
	return _root->get(key, default_value);
}


// Set an atomic-valued option.
template<class T>
void Params::put_atomic_value(const std::string &key, T value)
{
	load();
	_root->put(key, value);
	_ptree_saved = false;
}


// Convenience function to handle node key that holds a data different for LEFT and RIGHT.
std::string Params::side_key(Side side, const std::string &key)
{
	return (side==Side::LEFT ? "left." : "right.") + key;
}


