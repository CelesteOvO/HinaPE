// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_GRIDSYSTEMDATA2_JET_FBS_H_
#define FLATBUFFERS_GENERATED_GRIDSYSTEMDATA2_JET_FBS_H_

#include "flatbuffers/flatbuffers.h"

#include "basic_types_generated.h"

namespace jet
{
namespace fbs
{

struct ScalarGridSerialized2;

struct VectorGridSerialized2;

struct GridSystemData2;

struct ScalarGridSerialized2 FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table
{
    enum
    {
        VT_TYPE = 4, VT_DATA = 6
    };
    const flatbuffers::String *type() const
    {
        return GetPointer<const flatbuffers::String *>(VT_TYPE);
    }
    const flatbuffers::Vector<uint8_t> *data() const
    {
        return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_DATA);
    }
    bool Verify(flatbuffers::Verifier &verifier) const
    {
        return VerifyTableStart(verifier) && VerifyOffset(verifier, VT_TYPE) && verifier.Verify(type()) && VerifyOffset(verifier, VT_DATA) && verifier.Verify(data()) && verifier.EndTable();
    }
};

struct ScalarGridSerialized2Builder
{
    flatbuffers::FlatBufferBuilder &fbb_;
    flatbuffers::uoffset_t start_;
    void add_type(flatbuffers::Offset<flatbuffers::String> type)
    {
        fbb_.AddOffset(ScalarGridSerialized2::VT_TYPE, type);
    }
    void add_data(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data)
    {
        fbb_.AddOffset(ScalarGridSerialized2::VT_DATA, data);
    }
    ScalarGridSerialized2Builder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb)
    {
        start_ = fbb_.StartTable();
    }
    ScalarGridSerialized2Builder &operator=(const ScalarGridSerialized2Builder &);
    flatbuffers::Offset<ScalarGridSerialized2> Finish()
    {
        const auto end = fbb_.EndTable(start_, 2);
        auto o = flatbuffers::Offset<ScalarGridSerialized2>(end);
        return o;
    }
};

inline flatbuffers::Offset<ScalarGridSerialized2> CreateScalarGridSerialized2(flatbuffers::FlatBufferBuilder &_fbb, flatbuffers::Offset<flatbuffers::String> type = 0, flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data = 0)
{
    ScalarGridSerialized2Builder builder_(_fbb);
    builder_.add_data(data);
    builder_.add_type(type);
    return builder_.Finish();
}

inline flatbuffers::Offset<ScalarGridSerialized2> CreateScalarGridSerialized2Direct(flatbuffers::FlatBufferBuilder &_fbb, const char *type = nullptr, const std::vector<uint8_t> *data = nullptr)
{
    return jet::fbs::CreateScalarGridSerialized2(_fbb, type ? _fbb.CreateString(type) : 0, data ? _fbb.CreateVector<uint8_t>(*data) : 0);
}

struct VectorGridSerialized2 FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table
{
    enum
    {
        VT_TYPE = 4, VT_DATA = 6
    };
    const flatbuffers::String *type() const
    {
        return GetPointer<const flatbuffers::String *>(VT_TYPE);
    }
    const flatbuffers::Vector<uint8_t> *data() const
    {
        return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_DATA);
    }
    bool Verify(flatbuffers::Verifier &verifier) const
    {
        return VerifyTableStart(verifier) && VerifyOffset(verifier, VT_TYPE) && verifier.Verify(type()) && VerifyOffset(verifier, VT_DATA) && verifier.Verify(data()) && verifier.EndTable();
    }
};

struct VectorGridSerialized2Builder
{
    flatbuffers::FlatBufferBuilder &fbb_;
    flatbuffers::uoffset_t start_;
    void add_type(flatbuffers::Offset<flatbuffers::String> type)
    {
        fbb_.AddOffset(VectorGridSerialized2::VT_TYPE, type);
    }
    void add_data(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data)
    {
        fbb_.AddOffset(VectorGridSerialized2::VT_DATA, data);
    }
    VectorGridSerialized2Builder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb)
    {
        start_ = fbb_.StartTable();
    }
    VectorGridSerialized2Builder &operator=(const VectorGridSerialized2Builder &);
    flatbuffers::Offset<VectorGridSerialized2> Finish()
    {
        const auto end = fbb_.EndTable(start_, 2);
        auto o = flatbuffers::Offset<VectorGridSerialized2>(end);
        return o;
    }
};

inline flatbuffers::Offset<VectorGridSerialized2> CreateVectorGridSerialized2(flatbuffers::FlatBufferBuilder &_fbb, flatbuffers::Offset<flatbuffers::String> type = 0, flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data = 0)
{
    VectorGridSerialized2Builder builder_(_fbb);
    builder_.add_data(data);
    builder_.add_type(type);
    return builder_.Finish();
}

inline flatbuffers::Offset<VectorGridSerialized2> CreateVectorGridSerialized2Direct(flatbuffers::FlatBufferBuilder &_fbb, const char *type = nullptr, const std::vector<uint8_t> *data = nullptr)
{
    return jet::fbs::CreateVectorGridSerialized2(_fbb, type ? _fbb.CreateString(type) : 0, data ? _fbb.CreateVector<uint8_t>(*data) : 0);
}

struct GridSystemData2 FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table
{
    enum
    {
        VT_RESOLUTION = 4, VT_GRIDSPACING = 6, VT_ORIGIN = 8, VT_VELOCITYIDX = 10, VT_SCALARDATA = 12, VT_VECTORDATA = 14, VT_ADVECTABLESCALARDATA = 16, VT_ADVECTABLEVECTORDATA = 18
    };
    const jet::fbs::Size2 *resolution() const
    {
        return GetStruct<const jet::fbs::Size2 *>(VT_RESOLUTION);
    }
    const jet::fbs::Vector2D *gridSpacing() const
    {
        return GetStruct<const jet::fbs::Vector2D *>(VT_GRIDSPACING);
    }
    const jet::fbs::Vector2D *origin() const
    {
        return GetStruct<const jet::fbs::Vector2D *>(VT_ORIGIN);
    }
    uint64_t velocityIdx() const
    {
        return GetField<uint64_t>(VT_VELOCITYIDX, 0);
    }
    const flatbuffers::Vector<flatbuffers::Offset<ScalarGridSerialized2>> *scalarData() const
    {
        return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<ScalarGridSerialized2>> *>(VT_SCALARDATA);
    }
    const flatbuffers::Vector<flatbuffers::Offset<VectorGridSerialized2>> *vectorData() const
    {
        return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<VectorGridSerialized2>> *>(VT_VECTORDATA);
    }
    const flatbuffers::Vector<flatbuffers::Offset<ScalarGridSerialized2>> *advectableScalarData() const
    {
        return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<ScalarGridSerialized2>> *>(VT_ADVECTABLESCALARDATA);
    }
    const flatbuffers::Vector<flatbuffers::Offset<VectorGridSerialized2>> *advectableVectorData() const
    {
        return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<VectorGridSerialized2>> *>(VT_ADVECTABLEVECTORDATA);
    }
    bool Verify(flatbuffers::Verifier &verifier) const
    {
        return VerifyTableStart(verifier) && VerifyField<jet::fbs::Size2>(verifier, VT_RESOLUTION) && VerifyField<jet::fbs::Vector2D>(verifier, VT_GRIDSPACING) && VerifyField<jet::fbs::Vector2D>(verifier, VT_ORIGIN) && VerifyField<uint64_t>(verifier, VT_VELOCITYIDX) &&
               VerifyOffset(verifier, VT_SCALARDATA) && verifier.Verify(scalarData()) && verifier.VerifyVectorOfTables(scalarData()) && VerifyOffset(verifier, VT_VECTORDATA) && verifier.Verify(vectorData()) && verifier.VerifyVectorOfTables(vectorData()) &&
               VerifyOffset(verifier, VT_ADVECTABLESCALARDATA) && verifier.Verify(advectableScalarData()) && verifier.VerifyVectorOfTables(advectableScalarData()) && VerifyOffset(verifier, VT_ADVECTABLEVECTORDATA) && verifier.Verify(advectableVectorData()) &&
               verifier.VerifyVectorOfTables(advectableVectorData()) && verifier.EndTable();
    }
};

struct GridSystemData2Builder
{
    flatbuffers::FlatBufferBuilder &fbb_;
    flatbuffers::uoffset_t start_;
    void add_resolution(const jet::fbs::Size2 *resolution)
    {
        fbb_.AddStruct(GridSystemData2::VT_RESOLUTION, resolution);
    }
    void add_gridSpacing(const jet::fbs::Vector2D *gridSpacing)
    {
        fbb_.AddStruct(GridSystemData2::VT_GRIDSPACING, gridSpacing);
    }
    void add_origin(const jet::fbs::Vector2D *origin)
    {
        fbb_.AddStruct(GridSystemData2::VT_ORIGIN, origin);
    }
    void add_velocityIdx(uint64_t velocityIdx)
    {
        fbb_.AddElement<uint64_t>(GridSystemData2::VT_VELOCITYIDX, velocityIdx, 0);
    }
    void add_scalarData(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ScalarGridSerialized2>>> scalarData)
    {
        fbb_.AddOffset(GridSystemData2::VT_SCALARDATA, scalarData);
    }
    void add_vectorData(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<VectorGridSerialized2>>> vectorData)
    {
        fbb_.AddOffset(GridSystemData2::VT_VECTORDATA, vectorData);
    }
    void add_advectableScalarData(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ScalarGridSerialized2>>> advectableScalarData)
    {
        fbb_.AddOffset(GridSystemData2::VT_ADVECTABLESCALARDATA, advectableScalarData);
    }
    void add_advectableVectorData(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<VectorGridSerialized2>>> advectableVectorData)
    {
        fbb_.AddOffset(GridSystemData2::VT_ADVECTABLEVECTORDATA, advectableVectorData);
    }
    GridSystemData2Builder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb)
    {
        start_ = fbb_.StartTable();
    }
    GridSystemData2Builder &operator=(const GridSystemData2Builder &);
    flatbuffers::Offset<GridSystemData2> Finish()
    {
        const auto end = fbb_.EndTable(start_, 8);
        auto o = flatbuffers::Offset<GridSystemData2>(end);
        return o;
    }
};

inline flatbuffers::Offset<GridSystemData2> CreateGridSystemData2(flatbuffers::FlatBufferBuilder &_fbb, const jet::fbs::Size2 *resolution = 0, const jet::fbs::Vector2D *gridSpacing = 0, const jet::fbs::Vector2D *origin = 0, uint64_t velocityIdx = 0,
                                                                  flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ScalarGridSerialized2>>> scalarData = 0, flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<VectorGridSerialized2>>> vectorData = 0,
                                                                  flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ScalarGridSerialized2>>> advectableScalarData = 0,
                                                                  flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<VectorGridSerialized2>>> advectableVectorData = 0)
{
    GridSystemData2Builder builder_(_fbb);
    builder_.add_velocityIdx(velocityIdx);
    builder_.add_advectableVectorData(advectableVectorData);
    builder_.add_advectableScalarData(advectableScalarData);
    builder_.add_vectorData(vectorData);
    builder_.add_scalarData(scalarData);
    builder_.add_origin(origin);
    builder_.add_gridSpacing(gridSpacing);
    builder_.add_resolution(resolution);
    return builder_.Finish();
}

inline flatbuffers::Offset<GridSystemData2> CreateGridSystemData2Direct(flatbuffers::FlatBufferBuilder &_fbb, const jet::fbs::Size2 *resolution = 0, const jet::fbs::Vector2D *gridSpacing = 0, const jet::fbs::Vector2D *origin = 0, uint64_t velocityIdx = 0,
                                                                        const std::vector<flatbuffers::Offset<ScalarGridSerialized2>> *scalarData = nullptr, const std::vector<flatbuffers::Offset<VectorGridSerialized2>> *vectorData = nullptr,
                                                                        const std::vector<flatbuffers::Offset<ScalarGridSerialized2>> *advectableScalarData = nullptr, const std::vector<flatbuffers::Offset<VectorGridSerialized2>> *advectableVectorData = nullptr)
{
    return jet::fbs::CreateGridSystemData2(_fbb, resolution, gridSpacing, origin, velocityIdx, scalarData ? _fbb.CreateVector<flatbuffers::Offset<ScalarGridSerialized2>>(*scalarData) : 0,
                                           vectorData ? _fbb.CreateVector<flatbuffers::Offset<VectorGridSerialized2>>(*vectorData) : 0, advectableScalarData ? _fbb.CreateVector<flatbuffers::Offset<ScalarGridSerialized2>>(*advectableScalarData) : 0,
                                           advectableVectorData ? _fbb.CreateVector<flatbuffers::Offset<VectorGridSerialized2>>(*advectableVectorData) : 0);
}

inline const jet::fbs::GridSystemData2 *GetGridSystemData2(const void *buf)
{
    return flatbuffers::GetRoot<jet::fbs::GridSystemData2>(buf);
}

inline bool VerifyGridSystemData2Buffer(flatbuffers::Verifier &verifier)
{
    return verifier.VerifyBuffer<jet::fbs::GridSystemData2>(nullptr);
}

inline void FinishGridSystemData2Buffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<jet::fbs::GridSystemData2> root)
{
    fbb.Finish(root);
}

}  // namespace fbs
}  // namespace jet

#endif  // FLATBUFFERS_GENERATED_GRIDSYSTEMDATA2_JET_FBS_H_
