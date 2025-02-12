// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_BASICTYPES_JET_FBS_H_
#define FLATBUFFERS_GENERATED_BASICTYPES_JET_FBS_H_

#include "flatbuffers/flatbuffers.h"

namespace jet
{
namespace fbs
{

struct Size2;

struct Size3;

struct Vector2D;

struct Vector3D;

MANUALLY_ALIGNED_STRUCT(8) Size2 FLATBUFFERS_FINAL_CLASS
{ private:uint64_t x_;uint64_t y_;

public:Size2()
    {
        memset(this, 0, sizeof(Size2));
    }Size2(const Size2 &_o)
    {
        memcpy(this, &_o, sizeof(Size2));
    }Size2(uint64_t _x, uint64_t _y) : x_(flatbuffers::EndianScalar(_x)), y_(flatbuffers::EndianScalar(_y))
    {
    }uint64_t x() const
    {
        return flatbuffers::EndianScalar(x_);
    }uint64_t y() const
    {
        return flatbuffers::EndianScalar(y_);
    }};
STRUCT_END(Size2, 16);

MANUALLY_ALIGNED_STRUCT(8) Size3 FLATBUFFERS_FINAL_CLASS
{ private:uint64_t x_;uint64_t y_;uint64_t z_;

public:Size3()
    {
        memset(this, 0, sizeof(Size3));
    }Size3(const Size3 &_o)
    {
        memcpy(this, &_o, sizeof(Size3));
    }Size3(uint64_t _x, uint64_t _y, uint64_t _z) : x_(flatbuffers::EndianScalar(_x)), y_(flatbuffers::EndianScalar(_y)), z_(flatbuffers::EndianScalar(_z))
    {
    }uint64_t x() const
    {
        return flatbuffers::EndianScalar(x_);
    }uint64_t y() const
    {
        return flatbuffers::EndianScalar(y_);
    }uint64_t z() const
    {
        return flatbuffers::EndianScalar(z_);
    }};
STRUCT_END(Size3, 24);

MANUALLY_ALIGNED_STRUCT(8) Vector2D FLATBUFFERS_FINAL_CLASS
{ private:double x_;double y_;

public:Vector2D()
    {
        memset(this, 0, sizeof(Vector2D));
    }Vector2D(const Vector2D &_o)
    {
        memcpy(this, &_o, sizeof(Vector2D));
    }Vector2D(double _x, double _y) : x_(flatbuffers::EndianScalar(_x)), y_(flatbuffers::EndianScalar(_y))
    {
    }double x() const
    {
        return flatbuffers::EndianScalar(x_);
    }double y() const
    {
        return flatbuffers::EndianScalar(y_);
    }};
STRUCT_END(Vector2D, 16);

MANUALLY_ALIGNED_STRUCT(8) Vector3D FLATBUFFERS_FINAL_CLASS
{ private:double x_;double y_;double z_;

public:Vector3D()
    {
        memset(this, 0, sizeof(Vector3D));
    }Vector3D(const Vector3D &_o)
    {
        memcpy(this, &_o, sizeof(Vector3D));
    }Vector3D(double _x, double _y, double _z) : x_(flatbuffers::EndianScalar(_x)), y_(flatbuffers::EndianScalar(_y)), z_(flatbuffers::EndianScalar(_z))
    {
    }double x() const
    {
        return flatbuffers::EndianScalar(x_);
    }double y() const
    {
        return flatbuffers::EndianScalar(y_);
    }double z() const
    {
        return flatbuffers::EndianScalar(z_);
    }};
STRUCT_END(Vector3D, 24);

}  // namespace fbs
}  // namespace jet

#endif  // FLATBUFFERS_GENERATED_BASICTYPES_JET_FBS_H_
