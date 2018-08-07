#ifndef HLSL_SYNTAX
#define HLSL_SYNTAX

// Allows the preprocessor definitions for register
// slots to be easily used in the buffer definitions.


//----------------------------------------------------------------------------------
// Registers
//----------------------------------------------------------------------------------

#define REG_B(slot) register(b##slot)
#define REG_C(slot) register(c##slot)
#define REG_S(slot) register(s##slot)
#define REG_T(slot) register(t##slot)
#define REG_U(slot) register(u##slot)


#endif //HLSL_SYNTAX