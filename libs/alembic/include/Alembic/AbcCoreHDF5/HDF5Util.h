//-*****************************************************************************
//
// Copyright (c) 2009-2012,
//  Sony Pictures Imageworks, Inc. and
//  Industrial Light & Magic, a division of Lucasfilm Entertainment Company Ltd.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Sony Pictures Imageworks, nor
// Industrial Light & Magic nor the names of their contributors may be used
// to endorse or promote products derived from this software without specific
// prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//-*****************************************************************************

#ifndef _Alembic_AbcCoreHDF5_HDF5Util_h_
#define _Alembic_AbcCoreHDF5_HDF5Util_h_

#include <Alembic/AbcCoreHDF5/Foundation.h>
#include <Alembic/AbcCoreHDF5/HDF5Hierarchy.h>

namespace Alembic {
namespace AbcCoreHDF5 {
namespace ALEMBIC_VERSION_NS {

//-*****************************************************************************
typedef ::Alembic::Util::BaseDimensions<hsize_t> HDimensions;

//-*****************************************************************************
struct AttrCloser
{
    AttrCloser( hid_t id ) : m_id( id ) {}
    ~AttrCloser() { if ( m_id >= 0 ) H5Aclose( m_id ); }
    hid_t m_id;
};

//-*****************************************************************************
struct DspaceCloser
{
    DspaceCloser( hid_t id ) : m_id( id ) {}
    ~DspaceCloser() { if ( m_id >= 0 ) H5Sclose( m_id ); }
    hid_t m_id;
};

//-*****************************************************************************
struct DsetCloser
{
    DsetCloser( hid_t id ) : m_id( id ) {}
    ~DsetCloser() { if (m_id >= 0 ) H5Dclose( m_id ); }
    hid_t m_id;
};


//-*****************************************************************************
struct GroupCloser
{
    GroupCloser( hid_t id ) : m_id( id ) {}
    ~GroupCloser() { if ( m_id >= 0 ) H5Gclose( m_id ); }
    hid_t m_id;
};

//-*****************************************************************************
struct DtypeCloser
{
    DtypeCloser( hid_t id ) : m_id( id ) {}
    ~DtypeCloser() { if ( m_id >= 0 ) H5Tclose( m_id ); }
    hid_t m_id;
};

//-*****************************************************************************
struct PlistCloser
{
    PlistCloser( hid_t id ) : m_id( id ) {}
    ~PlistCloser() { if ( m_id >= 0 ) H5Pclose( m_id ); }
    hid_t m_id;
};

//-*****************************************************************************
hid_t CreationOrderPlist();
hid_t DsetGzipCreatePlist( const Dimensions &dims, int level );

//-*****************************************************************************
bool EquivalentDatatypes( hid_t idA, hid_t idB );

//-*****************************************************************************
H5Node OpenGroup( H5Node& iParent, const std::string& iName );

//-*****************************************************************************
void CloseObject (H5Node& iNode );

//-*****************************************************************************
bool GroupExists( H5Node& iParent, const std::string &iName );

//-*****************************************************************************
bool ObjectExists( H5Node& iParent, const std::string &iName );

//-*****************************************************************************
bool AttrExists( H5Node& iParent, const std::string &iName );

//-*****************************************************************************
bool DatasetExists( H5Node& iParent, const std::string &iName );

} // End namespace ALEMBIC_VERSION_NS

using namespace ALEMBIC_VERSION_NS;

} // End namespace AbcCoreHDF5
} // End namespace Alembic

#endif
