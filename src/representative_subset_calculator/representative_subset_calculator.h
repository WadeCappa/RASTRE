#include <vector>

class SubsetCalculator {
    public:
    
    virtual std::unique_ptr<Subset> getApproximationSet(std::unique_ptr<MutableSubset> consumer, const BaseData &data, size_t k) = 0; 

    std::unique_ptr<Subset> getApproximationSet(const BaseData &data, size_t k) {
        return this->getApproximationSet(move(NaiveMutableSubset::makeNew()), data, k);
    }
};