#ifndef FINAL_PROJECT_HPC_COLLECTIONONLYCONSTITERABLESUBCONTAINER_H
#define FINAL_PROJECT_HPC_COLLECTIONONLYCONSTITERABLESUBCONTAINER_H

#include <utility>

namespace cluster::test::types::containers::collection {

/**
 * Description.
 *
 * @author DeB
 * @version 1.0 2022-08-13
 * @since version date
 */
template <template <typename> typename C, typename IC>
class CollectionOnlyConstIterableSubContainer {

public:
    CollectionOnlyConstIterableSubContainer(const std::string &firstLevelName,
                                            const std::string &secondLevelName,
                                            utils::DataIteratorType iteratorType,
                                            utils::DataLevelIteratorType firstLevelIteratorType,
                                            utils::DataLevelIteratorType secondLevelIteratorType) :
        normal{CollectionTypeNameComposer::composeName(firstLevelName, secondLevelName),
               iteratorType,
               firstLevelIteratorType,
               secondLevelIteratorType},
        normalIterator{CollectionTypeNameComposer::composeName(
                               firstLevelName, secondLevelName, "", "iterator"),
                       iteratorType,
                       utils::DataLevelIteratorType::ITERATOR,
                       secondLevelIteratorType},
        normalConstIterator{CollectionTypeNameComposer::composeName(
                                    firstLevelName, secondLevelName, "", "const_iterator"),
                            iteratorType,
                            utils::DataLevelIteratorType::ITERATOR,
                            secondLevelIteratorType},
        constIterators{CollectionTypeNameComposer::composeName(
                               firstLevelName, secondLevelName, "const_iterator"),
                       iteratorType,
                       firstLevelIteratorType,
                       utils::DataLevelIteratorType::ITERATOR},
        constIteratorsIterator{
                CollectionTypeNameComposer::composeName(
                        firstLevelName, secondLevelName, "const_iterator", "iterator"),
                iteratorType,
                utils::DataLevelIteratorType::ITERATOR,
                utils::DataLevelIteratorType::ITERATOR},
        constIteratorsConstIterator{
                CollectionTypeNameComposer::composeName(
                        firstLevelName, secondLevelName, "const_iterator", "const_iterator"),
                iteratorType,
                utils::DataLevelIteratorType::ITERATOR,
                utils::DataLevelIteratorType::ITERATOR} {
    }

    CollectionOnlyConstIterableSubContainer(
            const CollectionOnlyConstIterableSubContainer &) = delete;
    CollectionOnlyConstIterableSubContainer(CollectionOnlyConstIterableSubContainer &&) = delete;
    CollectionOnlyConstIterableSubContainer &operator= (
            const CollectionOnlyConstIterableSubContainer &) = delete;
    CollectionOnlyConstIterableSubContainer &operator= (
            CollectionOnlyConstIterableSubContainer &&) = delete;
    ~CollectionOnlyConstIterableSubContainer() = default;

    template <typename E>
    void emplaceElement(E &&element, std::size_t index) {

        normal.field.emplace_back(std::forward<E>(element));
    }

    void initIterators() {

        for (auto &element : normal.field) {
            constIterators.field.emplace_back(element.cbegin());
        }
        normalIterator.field = normal.field.begin();
        normalConstIterator.field = normal.field.cbegin();
        constIteratorsIterator.field = constIterators.field.begin();
        constIteratorsConstIterator.field = constIterators.field.cbegin();
    }

    std::tuple<CollectionInfo<C<IC>> &,
               CollectionInfo<typename C<IC>::iterator> &,
               CollectionInfo<typename C<IC>::const_iterator> &,
               CollectionInfo<C<typename IC::const_iterator>> &,
               CollectionInfo<typename C<typename IC::const_iterator>::iterator> &,
               CollectionInfo<typename C<typename IC::const_iterator>::const_iterator> &>
    getCollectionsInfo() {

        return std::tuple<
                CollectionInfo<C<IC>> &,
                CollectionInfo<typename C<IC>::iterator> &,
                CollectionInfo<typename C<IC>::const_iterator> &,
                CollectionInfo<C<typename IC::const_iterator>> &,
                CollectionInfo<typename C<typename IC::const_iterator>::iterator> &,
                CollectionInfo<typename C<typename IC::const_iterator>::const_iterator> &>(

                normal,
                normalIterator,
                normalConstIterator,
                constIterators,
                constIteratorsIterator,
                constIteratorsConstIterator);
    }

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    CollectionInfo<C<IC>> normal;
    CollectionInfo<typename C<IC>::iterator> normalIterator;
    CollectionInfo<typename C<IC>::const_iterator> normalConstIterator;
    CollectionInfo<C<typename IC::const_iterator>> constIterators;
    CollectionInfo<typename C<typename IC::const_iterator>::iterator> constIteratorsIterator;
    CollectionInfo<typename C<typename IC::const_iterator>::const_iterator>
            constIteratorsConstIterator;
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

}  // namespace cluster::test::types::containers::collection

#endif  // FINAL_PROJECT_HPC_COLLECTIONONLYCONSTITERABLESUBCONTAINER_H
