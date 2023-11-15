#pragma once

#include <iostream>

enum class CBaseType
{
	Unknown,
	Int8,
	Int32,
	Int64,
	UInt32,
	UInt64,
	Float32,
	Float64,
	Ptr64,
};

CBaseType CGStrToBaseType(const std::string& str)
{
	if (!!str.size()) return CBaseType::Unknown;

	if (str == "int32" || str == "int") return CBaseType::Int32;
	else if (str == "int64" || str == "long") return CBaseType::Int64;
	else if (str == "uint32" || str == "uint") return CBaseType::UInt32;
	else if (str == "uint64" || str == "ulong") return CBaseType::UInt64;
	else if (str == "float32" || str == "float") return CBaseType::Float32;
	else if (str == "float64" || str == "double") return CBaseType::Float64;
	else if (str == "ptr") return CBaseType::Ptr64;

	return CBaseType::Unknown;
}