/*
 * VariablePy.h
 *
 *  Created on: Mar 13, 2017
 *      Author: wgodoy
 */

#ifndef VARIABLEPY_H_
#define VARIABLEPY_H_

#include "core/Variable.h"
#include "adiosPyFunctions.h"

namespace adios
{

template< class T>
class VariablePy : public Variable<T>
{

public:

	VariablePy<T>( const std::string name, const Dims dimensions, const Dims globalDimensions, const Dims globalOffsets,
            	const bool debugMode ):
        Variable<T>( name, dimensions, globalDimensions, globalOffsets, debugMode )
	{ }

	~VariablePy( )
	{ }

	void SetLocalDimensions( const boost::python::list list )
	{
		this->m_Dimensions = ListToVector( list );
	}

	std::vector<std::size_t> GetLocalDimensions( )
	{
		return this->m_Dimensions;
	}

};




}




#endif /* BINDINGS_PYTHON_INCLUDE_VARIABLEPY_H_ */
