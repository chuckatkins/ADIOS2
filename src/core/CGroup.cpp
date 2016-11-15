/*
 * CGroup.cpp
 *
 *  Created on: Oct 12, 2016
 *      Author: wfg
 */


#include <iostream>
#include <algorithm> // find
#include <sstream> //istringstream

#include "core/CGroup.h"
#include "core/SVariable.h" //for cast implementation of CVariableBase::Set that calls CVariable::Set
#include "public/SSupport.h"
#include "functions/ADIOSFunctions.h" //for XML Parsing functions (e.g. GetTag)


namespace adios
{


CGroup::CGroup( const std::string& hostLanguage, const bool debugMode ):
    m_HostLanguage{ hostLanguage },
    m_DebugMode{ debugMode }
{ }


CGroup::CGroup( const std::string& hostLanguage, const std::string& xmlGroup, const bool debugMode ):
    m_HostLanguage{ hostLanguage },
    m_DebugMode{ debugMode }
{
    ParseXMLGroup( xmlGroup );
}

CGroup::CGroup( const std::string& hostLanguage, const std::string transport,
                const unsigned int priority, const unsigned int iterations, const bool debugMode ):
    m_HostLanguage{ hostLanguage },
    m_DebugMode{ debugMode }
{ }


CGroup::~CGroup( )
{ }


void CGroup::CreateVariable( const std::string name, const std::string type,
                             const std::string dimensionsCSV, const std::string transform,
                             const std::string globalDimensionsCSV, const std::string globalOffsetsCSV )
{
    if( m_DebugMode == true )
    {
        if( SSupport::Datatypes.at( m_HostLanguage ).count( type ) == 0 )
            throw std::invalid_argument( "ERROR: type " + type + " for variable " + name + " is not supported.\n" );
        if( m_Variables.count( name ) == 0 ) //variable doesn't exists
            m_Variables[name] = std::make_pair( type, CurrentTypeIndex( type ) );
        else //name is found
            throw std::invalid_argument( "ERROR: variable " + name + " exists more than once.\n" );
    }
    else
    {
        m_Variables[name] = std::make_pair( type, CurrentTypeIndex( type ) );
    }

    int transformIndex = SetTransforms( transform );
    int globalBoundsIndex = SetGlobalBounds( globalDimensionsCSV, globalOffsetsCSV );

    if( type == "char" || type == "character" )
       m_Char.push_back( SVariable<char>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    else if( type == "unsigned char" )
        m_UChar.push_back( SVariable<unsigned char>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    else if( type == "short" || type == "integer*2" )
        m_Short.push_back( SVariable<short>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    else if( type == "unsigned short" )
        m_UShort.push_back( SVariable<unsigned short>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    else if( type == "int" || type == "integer" )
        m_Int.push_back( SVariable<int>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    else if( type == "unsigned int" || type == "unsigned integer" )
        m_UInt.push_back( SVariable<unsigned int>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    else if( type == "long int" || type == "long" || type == "long integer" )
        m_LInt.push_back( SVariable<long int>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    else if( type == "long long int" || type == "long long" || type == "long long integer" )
        m_LLInt.push_back( SVariable<long long int>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    else if( type == "unsigned long long int" || type == "unsigned long long" || type == "unsigned long long integer" )
        m_ULLInt.push_back( SVariable<unsigned long long int>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    else if( type == "float" || type == "real" || type == "real*4" )
        m_Float.push_back( SVariable<float>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    else if( type == "double" || type == "double precision" || type == "real*8" )
        m_Double.push_back( SVariable<double>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    else if( type == "long double" || type == "real*16" )
        m_LDouble.push_back( SVariable<long double>{ dimensionsCSV, nullptr, transformIndex, globalBoundsIndex } );

    m_SerialSize += name.size() + type.size() + dimensionsCSV.size() + transform.size() + 5 * sizeof(char); //5, adding one more for globalBoundsIndex
}


void CGroup::CreateAttribute( const std::string name, const std::string type, const std::string value,
                              const std::string globalDimensionsCSV, const std::string globalOffsetsCSV )
{
    auto lf_EmplaceVariable = [&]( const std::string name, const std::string type, const std::string value,
                                   const std::string globalDimensionsCSV, const std::string globalOffsetsCSV )
    {
        const int globalBoundsIndex = SetGlobalBounds( globalDimensionsCSV, globalOffsetsCSV );
        m_Attributes.emplace( name, SAttribute{ type, value, globalBoundsIndex } );
    };

    if( m_DebugMode == true )
    {
        if( m_Attributes.count( name ) == 0 ) //variable doesn't exists
            lf_EmplaceVariable( name, type, value, globalDimensionsCSV, globalOffsetsCSV );
        else //name is found
            throw std::invalid_argument( "ERROR: attribute " + name + " exists, NOT setting a new variable\n" );
    }
    else
    {
        lf_EmplaceVariable( name, type, value, globalDimensionsCSV, globalOffsetsCSV );
    }

    m_SerialSize += name.size() + type.size() + value.size() + 4; //adding one more for globalBoundsIndex
}


const unsigned long long int CGroup::GetIntVariableValue( const std::string variableName ) const
{
    if( m_DebugMode == true )
    {
        if( m_SetVariables.count( variableName ) == 0 )
            throw std::invalid_argument( "ERROR: variable value for " + variableName + " was not set with Write function\n" );
    }

    const std::string type( m_Variables.at( variableName ).first );
    const unsigned int index = m_Variables.at( variableName ).second;
    const unsigned long long int value = 0;

    if( type == "short" )
        value = *( m_Short[index].m_Values );

    else if( type == "unsigned short" )
        value = *( m_UShort[index].m_Values );

    else if( type == "int" )
        value = *( m_Int[index].m_Values );

    else if( type == "unsigned int" )
        value = *( m_UInt[index].m_Values );

    else if( type == "long int" )
        value = *( m_LInt[index].m_Values );

    else if( type == "unsigned long int" )
        value = *( m_ULInt[index].m_Values );

    else if( type == "long long int" )
        value = *( m_LLInt[index].m_Values );

    else if( type == "unsigned long long int" )
        value = *( m_ULLInt[index].m_Values );

    else
        throw std::invalid_argument( "ERROR: variable " + variableName + " must be of short, int or associated type (long int, unsigned long int, etc.)\n" );

    return value;
}


std::vector<unsigned long long int> CGroup::GetDimensions( const std::string dimensionsCSV ) const
{
    std::vector<unsigned long long int> dimensions;

    if( dimensionsCSV.find(',') == dimensionsCSV.npos ) //check if 1D
    {
        const std::string dimension( dimensionsCSV );
        dimensions.push_back( GetIntVariableValue( dimension ) );
        return dimensions;
    }

    std::istringstream dimensionsSS( dimensionsCSV );
    std::string dimension;
    while( std::getline( dimensionsSS, dimension, ',' ) )
    {
        dimensions.push_back( GetIntVariableValue( dimension ) );
    }

    return dimensions;
}


//PRIVATE FUNCTIONS BELOW
void CGroup::Monitor( std::ostream& logStream ) const
{
    logStream << "\tVariable \t Type\n";
    for( auto& variablePair : m_Variables )
    {
        logStream << "\t" << variablePair.first << " \t " << variablePair.second.first << "\n";
    }
    std::cout << "\n";

    logStream << "\tAttribute \t Type \t Value \n";
    for( auto& attributePair : m_Attributes )
    {
        logStream << "\t" << attributePair.first << " \t " << attributePair.second.Type << " \t " << attributePair.second.Value << "\n";
    }
    std::cout << "\n";
}


void CGroup::ParseXMLGroup( const std::string& xmlGroup )
{
    std::string::size_type currentPosition( 0 );
    std::string globalDimensionsCSV; //used to set variables
    std::string globalOffsetsCSV; //used to set variables

    while( currentPosition != std::string::npos )
    {
        //Get tag
        std::string tag;
        GetSubString( "<", ">", xmlGroup, tag, currentPosition );
        if( tag == "</adios-group>" ) break; //end of current group

        if( tag == "</global-bounds>" )
        {
            globalDimensionsCSV.clear(); //used for variables
            globalOffsetsCSV.clear(); //used for variables
        }

        if( m_DebugMode == true )
        {
            if( tag.size() < 2 )
                throw std::invalid_argument( "ERROR: wrong tag " + tag + " when reading group \n" ); //check < or <=)
        }
        tag = tag.substr( 1, tag.size() - 2 ); //eliminate < >

        //Get pairs from tag
        std::vector< std::pair<const std::string, const std::string> > pairs;
        GetPairsFromTag( xmlGroup, tag, pairs );

        //Check based on tagName
        const std::string tagName( tag.substr( 0, tag.find_first_of(" \t\n\r") ) );

        if( tagName == "var" ) //assign a Group variable
        {
            std::string name, type, transform, dimensionsCSV("1");

            for( auto& pair : pairs ) //loop through all pairs
            {
                if( pair.first == "name"       ) name = pair.second;
                else if( pair.first == "type"       ) type = pair.second;
                else if( pair.first == "dimensions" ) dimensionsCSV = pair.second;
                else if( pair.first == "transform"  ) transform = pair.second;
            }
            CreateVariable( name, type, dimensionsCSV, transform, globalDimensionsCSV, globalOffsetsCSV );
        }
        else if( tagName == "attribute" )
        {
            std::string name, path, value, type;
            for( auto& pair : pairs ) //loop through all pairs
            {
                if( pair.first == "name"       ) name = pair.second;
                else if( pair.first == "value" ) value = pair.second;
                else if( pair.first == "type"  ) type = pair.second;
            }
            CreateAttribute( name, type, value, globalDimensionsCSV, globalOffsetsCSV );
        }
        else if( tagName == "global-bounds" )
        {
            for( auto& pair : pairs ) //loop through all pairs
            {
                if( pair.first == "dimensions" )
                    globalDimensionsCSV = pair.second;
                else if( pair.first == "offsets" )
                    globalOffsetsCSV = pair.second;
            }

            if( m_DebugMode == true )
            {
                if( globalDimensionsCSV.empty() )
                    throw std::invalid_argument( "ERROR: dimensions missing in global-bounds tag\n");

                if( globalOffsetsCSV.empty() )
                    throw std::invalid_argument( "ERROR: offsets missing in global-bounds tag\n");
            }
        }
    } //end while loop
}


const unsigned int CGroup::CurrentTypeIndex( const std::string type ) const noexcept
{
    unsigned int index;

    if( type == "char" || type == "character" )
        index = m_Char.size();

    else if( type == "unsigned char" )
        index = m_UChar.size();

    else if( type == "short" || type == "integer*2" )
        index = m_Short.size();

    else if( type == "unsigned short" )
        index = m_UShort.size();

    else if( type == "int" || type == "integer" )
        index = m_Int.size();

    else if( type == "unsigned int" || type == "unsigned integer" )
        index = m_UInt.size();

    else if( type == "long int" || type == "long" || type == "long integer" )
        index = m_LInt.size();

    else if( type == "unsigned long int" || type == "unsigned long" || type == "unsigned long integer" )
        index = m_ULInt.size();

    else if( type == "long long int" || type == "long long" || type == "long long integer" )
        index = m_LLInt.size();

    else if( type == "unsigned long long int" || type == "unsigned long long" || type == "unsigned long long integer" )
        index = m_ULLInt.size();

    else if( type == "float" || type == "real" || type == "real*4" )
        index = m_Float.size();

    else if( type == "double" || type == "double precision" || type == "real*8" )
        index = m_Double.size();

    else if( type == "long double" || type == "real*16" )
        index = m_LDouble.size();

    return index;
}


const int CGroup::SetGlobalBounds( const std::string globalDimensionsCSV, const std::string globalOffsetsCSV ) noexcept
{
    if( globalDimensionsCSV.empty() || globalOffsetsCSV.empty() )
        return -1;

    int globalBoundsIndex = -1;
    const auto globalBounds = std::make_pair( globalDimensionsCSV, globalOffsetsCSV );
    auto itGlobalBounds = std::find( m_GlobalBounds.begin(), m_GlobalBounds.end(), globalBounds );

    if( itGlobalBounds != m_GlobalBounds.end() )
    {
        globalBoundsIndex = std::distance( m_GlobalBounds.begin(), itGlobalBounds );
    }
    else
    {
        m_GlobalBounds.push_back( globalBounds );
        globalBoundsIndex = m_GlobalBounds.size();
    }

    return globalBoundsIndex;
}


const int CGroup::SetTransforms( const std::string transform ) noexcept
{
    if( transform.empty() )
        return -1;

    int transformIndex = -1;
    auto itTransform = std::find( m_Transforms.begin(), m_Transforms.end(), transform );
    if( itTransform != m_Transforms.end() )
    {
        transformIndex = std::distance( m_Transforms.begin(), itTransform );
    }
    else
    {
        m_Transforms.push_back( transform );
        transformIndex = m_Transforms.size();
    }

    return transformIndex;
}


} //end namespace
