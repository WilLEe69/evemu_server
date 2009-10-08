/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2006 - 2008 The EVEmu Team
	For the latest information visit http://evemu.mmoforge.org
	------------------------------------------------------------------------------------
	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2 of the License, or (at your option) any later
	version.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place - Suite 330, Boston, MA 02111-1307, USA, or go to
	http://www.gnu.org/copyleft/lesser.txt.
	------------------------------------------------------------------------------------
	Author:		Zhur
*/


#ifndef __PYCALLABLE_H__
#define __PYCALLABLE_H__

#include "ServiceDB.h"

class Client;

class PyRep;
class PyTuple;
class PyDict;

class PyServiceMgr;
class PyCallStream;

class PyCallArgs {
public:
	PyCallArgs(Client *c, PyTuple* tup, PyDict* dict);
	~PyCallArgs();

	void Dump(LogType type) const;

	Client *const client;	//we do not own this
	PyTuple *tuple;		//we own this, but it may be taken
	std::map<std::string, PyRep *> byname;	//we own this, but elements may be taken.
};

class PyResult {
protected:
	PyResult();
public:
	PyResult(PyRep *result);	//takes ownership
	~PyResult();
	
	//I dislike smart pointers, but I need them here due to copying when returning objects
	counted_ptr<PyRep> ssResult;	//must never be NULL
};

class PyException {
protected:
	PyException();
public:
	PyException(PyRep *except);		//takes ownership
	~PyException();

	counted_ptr<PyRep> ssException;	//must never be NULL
};





class PyCallable {
public:
	class CallDispatcher {
	public:
		virtual ~CallDispatcher() {}

		virtual PyResult Dispatch(const std::string &method_name, PyCallArgs &call) = 0;
	};
	
	PyCallable();
	virtual ~PyCallable();

	//returns ownership:
	virtual PyResult Call(const std::string &method, PyCallArgs &args);
	
protected:
	void _SetCallDispatcher(CallDispatcher *d) { m_serviceDispatch = d; }

private:
	CallDispatcher *m_serviceDispatch;	//must not be NULL after constructor, we do not own this.
};

#endif // __PYCALLABLE_H__