#include <map>
#include <vector>
#include <iostream>
#include <memory>

static const std::string DELIMETER = ",";

class DataRow {
    public:
    virtual size_t size() const = 0;
    virtual void visit(DataRowVisitor &visitor) const = 0;
    virtual double dotProduct(const DataRow& dataRow) const = 0;
}; 

class DenseDataRow : public DataRow {
    private:
    std::vector<double> data;

    DenseDataRow(const DenseDataRow &);

    public:
    DenseDataRow() {}
    
    DenseDataRow(std::vector<double> input) : data(move(input)) {}

    static std::unique_ptr<DataRow> of(std::vector<double> data) {
        return std::unique_ptr<DataRow>(new DenseDataRow(move(data)));
    }

    void push_back(double val) {
        this->data.push_back(val);
    }

    size_t size() const {
        return this->data.size();
    }

    double dotProduct(const DataRow& dataRow) const {
        DenseDotProductDataRowVisitor visitor(this->data);
        dataRow.visit(visitor);
        return visitor.get().value();
    }

    void visit(DataRowVisitor &visitor) const {
        visitor.visitDenseDataRow(this->data);
    }
};

class SparseDataRow : public DataRow {
    private:
    const std::map<size_t, double> rowToValue;
    const size_t totalColumns;

    SparseDataRow(const SparseDataRow &);

    public:
    SparseDataRow(std::map<size_t, double> map, size_t totalColumns) : 
        rowToValue(move(map)), 
        totalColumns(totalColumns) 
    {}

    size_t size() const {
        return this->totalColumns;
    }

    double dotProduct(const DataRow& dataRow) const {
        SparseDotProductDataRowVisitor visitor(this->rowToValue);
        dataRow.visit(visitor);
        return visitor.get().value();
    }

    void visit(DataRowVisitor &visitor) const {
        visitor.visitSparseDataRow(this->rowToValue, this->totalColumns);
    }
};