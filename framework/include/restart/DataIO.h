/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef DATAIO_H
#define DATAIO_H

#include "Moose.h"
#include "ColumnMajorMatrix.h"

//libMesh
#include "libmesh/dense_matrix.h"
#include "libmesh/vector_value.h"
#include "libmesh/tensor_value.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class MooseMesh;
class FEProblem;

/**
 * Scalar helper routine
 */
template<typename P>
void storeHelper(std::ostream & stream, P & data);

/**
 * Vector helper routine
 */
template<typename P>
void storeHelper(std::ostream & stream, std::vector<P> & data);

/**
 * Scalar helper routine
 */
template<typename P>
void loadHelper(std::istream & stream, P & data);

/**
 * Vector helper routine
 */
template<typename P>
void loadHelper(std::istream & stream, std::vector<P> & data);


// global store functions

template<typename T>
void dataStore(std::ostream & stream, T & v)
{
  stream.write((char *) &v, sizeof(v));
}

template<typename T>
void dataStore(std::ostream & stream, T * & v)
{
  mooseError("Cannot store raw pointers as restartable data!\nWrite a custom dataStore() template specialization!\n\n");
}

template<>
inline
void dataStore(std::ostream & stream, Real & v)
{
  stream.write((char *) &v, sizeof(v));
//  std::cout<<"value: "<<v<<std::endl;
}

template<typename T>
inline void
vectorDataStore(std::ostream & stream, std::vector<T> & v)
{
  // First store the size of the vector
  unsigned int size = v.size();
  stream.write((char *) &size, sizeof(size));

  for (unsigned int i = 0; i < size; i++)
    storeHelper(stream, v[i]);
}

template<>
inline void
dataStore(std::ostream & stream, DenseMatrix<Real> & v)
{
  for (unsigned int i = 0; i < v.m(); i++)
    for (unsigned int j = 0; j < v.n(); j++)
    {
      Real r = v(i, j);
      stream.write((char *) &r, sizeof(r));
    }
}

template<>
inline void
dataStore(std::ostream & stream, ColumnMajorMatrix & v)
{
  for (unsigned int i = 0; i < v.m(); i++)
    for (unsigned int j = 0; j < v.n(); j++)
    {
      Real r = v(i, j);
      stream.write((char *) &r, sizeof(r));
    }
}

template<>
inline void
dataStore(std::ostream & stream, RealTensorValue & v)
{
  for (unsigned int i = 0; i < LIBMESH_DIM; i++)
    for (unsigned int j = 0; i < LIBMESH_DIM; i++)
      stream.write((char *) &v(i, j), sizeof(v(i, j)));
}

template<>
inline void
dataStore(std::ostream & stream, RealVectorValue & v)
{
  // Obviously if someone loads data with different LIBMESH_DIM than was used for saving them, it won't work.
  for (unsigned int i = 0; i < LIBMESH_DIM; i++)
    stream.write((char *) &v(i), sizeof(v(i)));
}


// global load functions

template<typename T>
void dataLoad(std::istream & stream, T & v)
{
  stream.read((char *) &v, sizeof(v));
}

template<typename T>
void dataLoad(std::istream & stream, T * & v)
{
  mooseError("Cannot load raw pointers as restartable data!\nWrite a custom dataLoad() template specialization!\n\n");
}

template<>
inline void
dataLoad(std::istream & stream, Real & v)
{
  stream.read((char *) &v, sizeof(v));
//  std::cout<<"value: "<<v<<std::endl;
}

template<typename T>
inline void
vectorDataLoad(std::istream & stream, std::vector<T> & v)
{
  // First read the size of the vector
  unsigned int size = 0;
  stream.read((char *) &size, sizeof(size));

  v.resize(size);

  for (unsigned int i = 0; i < size; i++)
    loadHelper(stream, v[i]);
}

template<>
inline void
dataLoad(std::istream & stream, DenseMatrix<Real> & v)
{
  for (unsigned int i = 0; i < v.m(); i++)
    for (unsigned int j = 0; j < v.n(); j++)
    {
      Real r = 0;
      stream.read((char *) &r, sizeof(r));
      v(i, j) = r;
    }
}

template<>
inline void
dataLoad(std::istream & stream, ColumnMajorMatrix & v)
{
  for (unsigned int i = 0; i < v.m(); i++)
    for (unsigned int j = 0; j < v.n(); j++)
    {
      Real r = 0;
      stream.read((char *) &r, sizeof(r));
      v(i, j) = r;
    }
}

template<>
inline void
dataLoad(std::istream & stream, RealTensorValue & v)
{
  // Obviously if someone loads data with different LIBMESH_DIM than was used for saving them, it won't work.
  for (unsigned int i = 0; i < LIBMESH_DIM; i++)
    for (unsigned int j = 0; i < LIBMESH_DIM; i++)
    {
      Real r = 0;
      stream.read((char *) &r, sizeof(r));
      v(i, j) = r;
    }
}

template<>
inline void
dataLoad(std::istream & stream, RealVectorValue & v)
{
  // Obviously if someone loads data with different LIBMESH_DIM than was used for saving them, it won't work.
  for (unsigned int i = 0; i < LIBMESH_DIM; i++)
  {
    Real r = 0;
    stream.read((char *) &r, sizeof(r));
    v(i) = r;
  }
}

// Scalar Helper Function
template<typename P>
void storeHelper(std::ostream & stream, P & data)
{
  dataStore(stream, data);
}

// Vector Helper Function
template<typename P>
void storeHelper(std::ostream & stream, std::vector<P> & data)
{
  vectorDataStore(stream, data);
}

// Scalar Helper Function
template<typename P>
void loadHelper(std::istream & stream, P & data)
{
  dataLoad(stream, data);
}

// Vector Helper Function
template<typename P>
void loadHelper(std::istream & stream, std::vector<P> & data)
{
  vectorDataLoad(stream, data);
}

#endif /* DATAIO_H */
