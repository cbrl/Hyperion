#ifndef HLSL_SYNTAX
#define HLSL_SYNTAX


//----------------------------------------------------------------------------------
// Registers
//----------------------------------------------------------------------------------

#define REG_B(slot) register(b##slot)
#define REG_C(slot) register(c##slot)
#define REG_S(slot) register(s##slot)
#define REG_T(slot) register(t##slot)
#define REG_U(slot) register(u##slot)


//----------------------------------------------------------------------------------
// Utility
//----------------------------------------------------------------------------------

#define CAT(a, b) a##b


#endif //HLSL_SYNTAX