#include "Enums.hpp"

int AttributeSize(attribute_type attr) {
    switch (attr) {
    case attribute_type::INT8:
    case attribute_type::UINT8:
        return 1;
    case attribute_type::INT16:
    case attribute_type::UINT16:
    case attribute_type::FLOAT16:
        return 2;
    case attribute_type::INT32:
    case attribute_type::UINT32:
    case attribute_type::FLOAT32:
        return 4;
    case attribute_type::FLOAT64:
        return 8;
    }
}