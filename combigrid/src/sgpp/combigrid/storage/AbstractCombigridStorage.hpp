// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#ifndef COMBIGRID_SRC_SGPP_COMBIGRID_STORAGE_ABSTRACTCOMBIGRIDSTORAGE_HPP_
#define COMBIGRID_SRC_SGPP_COMBIGRID_STORAGE_ABSTRACTCOMBIGRIDSTORAGE_HPP_

#include <sgpp/combigrid/common/MultiIndexIterator.hpp>
#include <sgpp/combigrid/definitions.hpp>
#include <sgpp/combigrid/storage/AbstractMultiStorageIterator.hpp>

#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace sgpp {
namespace combigrid {

/**
 * Abstract base class for storage storing a float_t for each level-index-pair. The intention is
 * that the entries store function values to be re-used. In the subclass CombigridTreeStorage, a
 * function is passed whose values are evaluated and stored at the first request. At further
 * requests for the same level-index pair, the stored value can be retrieved.
 */
class AbstractCombigridStorage {
 public:
  virtual ~AbstractCombigridStorage();

  /**
   * @param level Level to traverse
   * @param iterator Iterator that defines which values should be iterated over
   * @param orderingConfiguration Defines for each dimension whether the grid points in that
   * dimension should be traversed in sorted order.
   * @return Returns an iterator that iterates along the multi-indices for a given level. These
   * multi-indices are given by the parameter iterator.
   * When moveToNext() is called on the returned iterator, it also calls moveToNext() on the
   * underlying MultiIndexIterator.
   * If the values are not already stored, they are created during iteration.
   */
  virtual std::shared_ptr<AbstractMultiStorageIterator<double>> getGuidedIterator(
      MultiIndex const &level, MultiIndexIterator &iterator,
      std::vector<bool> orderingConfiguration) = 0;

  /**
   * @return Returns the number of stored values (accumulated over all levels).
   */
  virtual size_t getNumEntries() = 0;

  /**
   * @return Returns a string that contains all necessary information to reconstruct the stored
   * values.
   */
  virtual std::string serialize() = 0;

  /**
   * Re-loads storage values from a string that has been generated by serialize().
   */
  virtual void deserialize(std::string const &str) = 0;

  /**
   * Sets a value at the given level-index pair, creating a new entry if no value had been
   * previously stored at this level-index pair.
   */
  virtual void set(MultiIndex const &level, MultiIndex const &index, double value) = 0;

  virtual double get(MultiIndex const &level, MultiIndex const &index) = 0;

  /**
   * Sets a mutex that is locked for critical operations. If the mutex is nullptr, nothing is
   * locked.
   */
  virtual void setMutex(std::shared_ptr<std::recursive_mutex> mutexPtr) = 0;
};

} /* namespace combigrid */
} /* namespace sgpp*/

#endif /* COMBIGRID_SRC_SGPP_COMBIGRID_STORAGE_ABSTRACTCOMBIGRIDSTORAGE_HPP_ */