// Helper to extract public const (literal) field values from managed assemblies
// Minimal implementation: supports integer (I4/I8), unsigned (U4/U8), boolean, float (R4) and double (R8).

#include "stdafx.h"
#include "Core.h"

extern "C" bool CLR_ExtractPublicConst_Numeric(const char *szClass, const char *szField, void *outValue, int outSize)
{
    NATIVE_PROFILE_CLR_CORE();

    if (!szClass || !szField || !outValue)
        return false;

    CLR_RT_TypeDef_Index tdIdx{};

    if (!g_CLR_RT_TypeSystem.FindTypeDef(szClass, tdIdx))
        return false;

    CLR_RT_TypeDef_Instance tdInst{};
    if (!tdInst.InitializeFromIndex(tdIdx))
        return false;

    CLR_RT_FieldDef_Index fdIdx{};
    if (!tdInst.m_assm->FindFieldDef(tdInst.m_target, szField, NULL, CLR_SIG_INVALID, fdIdx))
        return false;

    CLR_RT_FieldDef_Instance fdInst{};
    if (!fdInst.InitializeFromIndex(fdIdx))
        return false;

    // Must be a literal (compile-time constant)
    if ((fdInst.m_target->flags & CLR_RECORD_FIELDDEF::FD_Literal) == 0)
        return false;

    if (fdInst.m_target->defaultValue == CLR_EmptyIndex)
        return false;

    CLR_PMETADATA ptr = fdInst.m_assm->GetSignature(fdInst.m_target->defaultValue);

    // Determine field type
    CLR_RT_SignatureParser parser{};
    parser.Initialize_FieldDef(fdInst.m_assm, fdInst.m_target);
    CLR_RT_SignatureParser::Element res{};
    if (parser.Advance(res) != S_OK)
        return false;

    // Read raw bytes according to data type
    switch (res.m_dt)
    {
        case DATATYPE_BOOLEAN:
        case DATATYPE_I1:
        case DATATYPE_U1:
        {
            CLR_UINT8 v8 = 0;
            NANOCLR_READ_UNALIGNED_UINT8(v8, ptr);
            if (outSize >= (int)sizeof(CLR_UINT8))
                memcpy(outValue, &v8, sizeof(CLR_UINT8));
            return true;
        }
        case DATATYPE_CHAR:
        case DATATYPE_I2:
        case DATATYPE_U2:
        {
            CLR_UINT16 v16 = 0;
            NANOCLR_READ_UNALIGNED_UINT16(v16, ptr);
            if (outSize >= (int)sizeof(CLR_UINT16))
                memcpy(outValue, &v16, sizeof(CLR_UINT16));
            return true;
        }
        case DATATYPE_I4:
        case DATATYPE_U4:
        {
            CLR_INT32 v32 = 0;
            NANOCLR_READ_UNALIGNED_INT32(v32, ptr);
            if (outSize >= (int)sizeof(CLR_INT32))
                memcpy(outValue, &v32, sizeof(CLR_INT32));
            return true;
        }
        case DATATYPE_I8:
        case DATATYPE_U8:
        case DATATYPE_DATETIME:
        case DATATYPE_TIMESPAN:
        {
            CLR_INT64 v64 = 0;
            NANOCLR_READ_UNALIGNED_INT64(v64, ptr);
            if (outSize >= (int)sizeof(CLR_INT64))
                memcpy(outValue, &v64, sizeof(CLR_INT64));
            return true;
        }
        case DATATYPE_R4:
        {
            CLR_UINT32 tmp = 0;
            NANOCLR_READ_UNALIGNED_UINT32(tmp, ptr);
            if (outSize >= (int)sizeof(float))
            {
                float f;
                memcpy(&f, &tmp, sizeof(float));
                memcpy(outValue, &f, sizeof(float));
            }
            return true;
        }
        case DATATYPE_R8:
        {
            CLR_UINT64 tmp = 0;
            NANOCLR_READ_UNALIGNED_UINT64(tmp, ptr);
            if (outSize >= (int)sizeof(double))
            {
                double d;
                memcpy(&d, &tmp, sizeof(double));
                memcpy(outValue, &d, sizeof(double));
            }
            return true;
        }
        default:
            // Unsupported: strings, objects, arrays, etc.
            return false;
    }

    return false;
}
