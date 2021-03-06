/*
 *  vp_manager.h
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef VP_MANAGER_H
#define VP_MANAGER_H

// Includes from libnestutil:
#include "manager_interface.h"

// Includes from nestkernel:
#include "nest_types.h"

// Includes from sli:
#include "dictdatum.h"

#ifdef _OPENMP
// C includes:
#include <omp.h>
#endif

namespace nest
{

class VPManager : public ManagerInterface
{
public:
  VPManager();
  ~VPManager()
  {
  }

  virtual void initialize();
  virtual void finalize();

  virtual void set_status( const DictionaryDatum& );
  virtual void get_status( DictionaryDatum& );

  /**
   * Gets ID of local thread.
   * Returns thread ID if OPENMP is installed
   * and zero otherwise.
   */
  thread get_thread_id() const;

  /**
   * Set the number of threads by setting the internal variable
   * n_threads_, the corresponding value in the Communicator, and
   * the OpenMP number of threads.
   */
  void set_num_threads( thread n_threads );

  /**
   * Get number of threads.
   * This function returns the total number of threads per process.
   */
  index get_num_threads() const;

  /**
   * Return a thread number for a given global node id.
   * Each node has a default thread on which it will run.
   * The thread is defined by the relation:
   * t = (gid div P) mod T, where P is the number of simulation processes and
   * T the number of threads. This may be used by Network::add_node()
   * if the user has not specified anything.
   */
  thread suggest_vp( index ) const;

  /**
   * Return a thread number for a given global recording node id.
   * Each node has a default thread on which it will run.
   * The thread is defined by the relation:
   * t = (gid div P) mod T, where P is the number of recording processes and
   * T the number of threads. This may be used by Network::add_node()
   * if the user has not specified anything.
   */
  thread suggest_rec_vp( index ) const;

  /**
   * Convert a given VP ID to the corresponding thread ID
   */
  thread vp_to_thread( thread vp ) const;

  /**
   * Convert a given thread ID to the corresponding VP ID
   */
  thread thread_to_vp( thread t ) const;

  /**
   * Return true, if the given VP is on the local machine
   */
  bool is_local_vp( thread ) const;

  /**
   * Returns the number of virtual processes.
   */
  int get_num_virtual_processes() const;

  /**
   * Fails if NEST is in thread-parallel section.
   */
  static void assert_single_threaded();

private:
  const bool force_singlethreading_;
  index n_threads_; //!< Number of threads per process.
};
}

inline nest::thread
nest::VPManager::get_thread_id() const
{
#ifdef _OPENMP
  return omp_get_thread_num();
#else
  return 0;
#endif
}

inline nest::index
nest::VPManager::get_num_threads() const
{
  return n_threads_;
}

#endif /* VP_MANAGER_H */
