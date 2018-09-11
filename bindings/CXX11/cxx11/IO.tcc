/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * IO.tcc :
 *
 *  Created on: Jun 4, 2018
 *      Author: William F Godoy godoywf@ornl.gov
 */
#ifndef ADIOS2_BINDINGS_CXX11_CXX11_IO_TCC_
#define ADIOS2_BINDINGS_CXX11_CXX11_IO_TCC_

#include "IO.h"

#include "adios2/core/IO.h"

namespace adios2
{

template <class T>
Variable<T> IO::DefineVariable(const std::string &name, const Dims &shape,
                               const Dims &start, const Dims &count,
                               const bool constantDims)
{
    return Variable<T>(
        &m_IO.DefineVariable<T>(name, shape, start, count, constantDims));
}

template <class T>
Variable<T> IO::InquireVariable(const std::string &name) noexcept
{
    return Variable<T>(m_IO.InquireVariable<T>(name));
}

template <class T>
Attribute<T> IO::DefineAttribute(const std::string &name, const T *data,
                                 const size_t size)
{
    return Attribute<T>(&m_IO.DefineAttribute(name, data, size));
}

template <class T>
Attribute<T> IO::DefineAttribute(const std::string &name, const T &value)
{
    return Attribute<T>(&m_IO.DefineAttribute<T>(name, value));
}

template <class T>
Attribute<T> IO::DefineAttribute(const adios2::Variable<T> variable,
                                 const std::string &name, const T &value,
                                 const std::string separator)
{
    if (!variable)
    {
        throw std::invalid_argument(
            "ERROR: variable can't be false when "
            "defining related attribute value with name " +
            name + ", in call to DefineAttribute\n");
    }

    const std::string absoluteName = variable.m_Name + separator + name;
    return DefineAttribute(absoluteName, value, separator);
}

template <class T>
Attribute<T> IO::DefineAttribute(const adios2::Variable<T> variable,
                                 const std::string &name, const T *data,
                                 const size_t size, const std::string separator)
{
    if (!variable)
    {
        throw std::invalid_argument(
            "ERROR: variable can't be false when "
            "defining related attribute array with name " +
            name + ", in call to DefineAttribute\n");
    }

    const std::string absoluteName = variable.m_Name + separator + name;
    return DefineAttribute(absoluteName, data, size);
}

template <class T>
Attribute<T> IO::InquireAttribute(const std::string &name) noexcept
{
    return Attribute<T>(m_IO.InquireAttribute<T>(name));
}

template <class T>
Attribute<T> IO::InquireAttribute(const Variable<T> variable,
                                  const std::string &name,
                                  const std::string separator) noexcept
{
    if (!variable)
    {
        throw std::invalid_argument("ERROR: variable can't be false when "
                                    "inquiring related attribute with name " +
                                    name + ", in call to DefineAttribute\n");
    }

    const std::string absoluteName = variable.m_Name + separator + name;
    return InquireAttribute<T>(absoluteName);
}

} // end namespace adios2

#endif /* ADIOS2_BINDINGS_CXX11_CXX11_IO_TCC_ */
