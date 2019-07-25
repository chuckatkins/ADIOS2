*****************************************
Building, Testing, and Installing ADIOS 2
*****************************************

.. caution::

   Always do a fresh build from scratch if your source is updated with considerable changes *e.g.* `git pull` 


To build ADIOS 2, clone the repository and invoke the canonical CMake build sequence:

.. code-block:: bash

    $ git clone https://github.com/ornladios/ADIOS2.git ADIOS2
    $ mkdir adios2-build && cd adios2-build
    $ cmake ../ADIOS2
    -- The C compiler identification is GNU 7.3.0
    -- The CXX compiler identification is GNU 7.3.0
    ...

    ADIOS2 build configuration:
      ...
      Features:
        Library Type: shared
        Build Type:   Release
        Testing: ON
        Build Options:
          BZip2    : ON
          ZFP      : OFF
          SZ       : OFF
          MGARD    : OFF
          MPI      : ON
          DataMan  : ON
          SST      : ON
          ZeroMQ   : ON
          HDF5     : ON
          Python   : ON
          Fortran  : ON
          SysVShMem: ON
          Endian_Reverse: OFF


Then compile using

.. code-block:: bash

    $ make -j 16

Optionally, run the tests:

.. code-block:: bash

    $ ctest
    Test project /home/wgodoy/workspace/build
            Start   1: ADIOSInterfaceWriteTest.DefineVar_int8_t_1x10
      1/295 Test   #1: ADIOSInterfaceWriteTest.DefineVar_int8_t_1x10 .........................   Passed    0.16 sec
            Start   2: ADIOSInterfaceWriteTest.DefineVar_int16_t_1x10
      2/295 Test   #2: ADIOSInterfaceWriteTest.DefineVar_int16_t_1x10 ........................   Passed    0.06 sec
            Start   3: ADIOSInterfaceWriteTest.DefineVar_int32_t_1x10

          ...

            Start 294: ADIOSBZip2Wrapper.WrongParameterValue
    294/295 Test #294: ADIOSBZip2Wrapper.WrongParameterValue .................................   Passed    0.00 sec
            Start 295: ADIOSBZip2Wrapper.WrongBZip2Name
    295/295 Test #295: ADIOSBZip2Wrapper.WrongBZip2Name ......................................   Passed    0.00 sec

    100% tests passed, 0 tests failed out of 295

    Total Test time (real) =   95.95 sec


And finally, use the standard invocation to install:

.. code-block:: bash

    $ make install


*************
CMake Options
*************

The following options can be specified with CMake's ``-DVAR=VALUE`` syntax. The default option is highlighted.

============================= ================ ==========================================================================================================================================================================================================================
VAR                            VALUE                     Description
============================= ================ ==========================================================================================================================================================================================================================
``ADIOS2_USE_MPI``             **ON**/OFF      MPI or non-MPI (serial) build.
``ADIOS2_USE_ZeroMQ``          **ON**/OFF      `ZeroMQ <http://zeromq.org/>`_ for the DataMan engine.
``ADIOS2_USE_HDF5``            **ON**/OFF      `HDF5 <https://www.hdfgroup.org>`_ engine. If HDF5 is not on the syspath, it can be set using ``-DHDF5_ROOT=/path/to/hdf5``
``ADIOS2_USE_Python``          **ON**/OFF      Python bindings. Python 3 will be used if found. If you want to specify a particular python version use  ``-DPYTHON_EXECUTABLE=/path/to/interpreter/python``
``ADIOS2_USE_Fortran``         **ON**/OFF      Bindings for Fortran 90 or above.
``ADIOS2_USE_SST``             **ON**/OFF      Simplified Staging Engine (SST) and its dependencies, requires MPI. Can optionally use LibFabric for RDMA transport. Specify the LibFabric install manually with the -DLIBFABRIC_ROOT=... option.
``ADIOS2_USE_BZip2``           **ON**/OFF      `BZIP2 <http://www.bzip.org>`_ compression.
``ADIOS2_USE_ZFP``             **ON**/OFF      `ZFP <https://github.com/LLNL/zfp>`_ compression (experimental).
``ADIOS2_USE_SZ``              **ON**/OFF      `SZ <https://github.com/disheng222/SZ>`_ compression (experimental).
``ADIOS2_USE_MGARD``           **ON**/OFF      `MGARD <https://github.com/CODARcode/MGARD>`_ compression (experimental).
``ADIOS2_USE_PNG``             **ON**/OFF      `PNG <https://libpng.org>`_ compression (experimental).
``ADIOS2_USE_Blosc``           **ON**/OFF      `Blosc <http://blosc.org/>`_ compression (experimental).
``ADIOS2_USE_Endian_Reverse``  ON/**OFF**      Big/Little Endian Interoperability for different endianness platforms at write and read.
============================= ================ ==========================================================================================================================================================================================================================

In addition to the ``ADIOS2_USE_Feature`` options, the following options are also available to control how the library gets built:

==================================== =============================================== ===============================
 CMake VAR Options                       Values                                       Description                                                                          |
==================================== =============================================== ===============================
``BUILD_SHARED_LIBS``                  **ON**/OFF                                     Build shared libraries.
``ADIOS2_BUILD_EXAMPLES``              **ON**/OFF                                     Build examples.
``ADIOS2_BUILD_TESTING``               **ON**/OFF                                     Build test code.
``CMAKE_INSTALL_PREFIX``               /path/to/install (``/usr/local``)              Installation location.
``CMAKE_BUILD_TYPE``                   Debug/**Release**/RelWithDebInfo/MinSizeRel    Compiler optimization levels.
==================================== =============================================== ===============================


Example: Enable Fortran, disable Python bindings and ZeroMQ functionality

.. code-block:: bash

    $ cmake -DADIOS2_USE_Fortran=ON -DADIOS2_USE_Python=OFF -DADIOS2_USE_ZeroMQ=OFF ../ADIOS2


Notes:
   * The ``ADIOS2_USE_HDF5`` option requires the use of a matching serial or parallel version depending on whether ``ADIOS2_USE_MPI`` is enabled. Similary, enabling MPI and Python bindings require ``mpi4py``.

   * Optional ROOT suffix to a dependency can guide ``cmake`` into finding a particular dependency:

.. code-block:: bash

    $ cmake -DHDF5_ROOT=/opt/hdf5/1.12.0 ../ADIOS2

Example: the following configuration will build, test and install under /opt/adios2/2.4.0 an optimized (Release) version of ADIOS2.

.. code-block:: bash

    $ cd build
    $ cmake -DADIOS2_USE_Fortran=ON -DCMAKE_INSTALL_PREFIX=/opt/adios2/2.4.0 -DCMAKE_BUILD_Type=Release ../ADIOS2
    $ make -j16
    $ ctest
    $ make install

For a fully configurable build script, click `here. <https://github.com/ornladios/ADIOS2/tree/master/scripts/runconf/runconf.sh>`_