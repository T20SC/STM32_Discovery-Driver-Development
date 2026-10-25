/*
 * bitmath.h
 *
 *  Created on: Sep 22, 2026
 *      Author: tmt
 */

#ifndef BITMATH_H_
#define BITMATH_H_


#define SET_BIT(REG, BIT_NO)  ( (REG) = (REG) | (1U<<(BIT_NO)) )
/*
    PURPOSE: *** Sets the wanted bit to 1 without changing the rest ***
    MASK:(1<<BIT_NO)
    |:Bitwise operation OR
*/

#define CLR_BIT(REG,BIT_NO)  ( (REG) =  (REG) & ~(1U<<(BIT_NO) ) )
 /*
    PURPOSE:*** Sets the wanted bit to 0 without changing the rest ***
    MASK:(1<<BIT_NO)
    ~:Reverses Bits (1->0 , 0->1) (1's Complement)
    &:Mask operation ANDed with REG
*/

#define TOG_BIT(REG,BIT_NO) ( (REG) = (REG) ^ (1U<<(BIT_NO) ) )
/*
    PURPOSE:*** Flips/Inverts the wanted bit from 0 to 1 or vice versa
    MASK:(1<<BIT_NO)
    ^:Bitwise XOR (0:0->0 , 0:1->1 ,1:0->1 , 1:1->0)
*/

#define GET_BIT(REG,BIT_NO)   (1U & ((REG) >> (BIT_NO)))
/*
    PURPOSE:*** Doesn't Change Data Only reads it
    MASK:(REG>>BIT_NO) shifts the bits of REG by BIT_NO to the Right
    &:Bitwise AND Operation between the MASK and 1 -> (0b00000001)
*/

/* mask with ones in bits HIGH..LOW (inclusive) */
#define FIELD_MASK(HIGH, LOW)   ((0xFFFFFFFFU >> (31U - ((HIGH) - (LOW)))) << (LOW))

/* set every bit of the field to 1, rest untouched */
#define SET_FIELD(REG, HIGH, LOW)   ((REG) |= FIELD_MASK(HIGH, LOW))

/* clear every bit of the field to 0, rest untouched */
#define CLR_FIELD(REG, HIGH, LOW)   ((REG) &= ~FIELD_MASK(HIGH, LOW))

/*Gets the bit field from bit A to bit B*/
#define GET_FIELD(REG, HIGH, LOW)   (((REG) & FIELD_MASK(HIGH, LOW)) >> (LOW)) 

/* write a value into the field, rest untouched */
#define WRITE_FIELD(REG, HIGH, LOW, VAL) \
    ((REG) = ((REG) & ~FIELD_MASK(HIGH, LOW)) | (((uint32_t)(VAL) << (LOW)) & FIELD_MASK(HIGH, LOW)))

#endif /* BITMATH_H_ */
