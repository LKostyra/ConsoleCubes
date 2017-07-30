#include "PCH.hpp"
#include "BlockLibrary.hpp"

namespace Blocks
{

unsigned char longShape[]    = { 1,
                                 1,
                                 1,
                                 1, };

Block Long({ {longShape, 1, 4}, {longShape, 4, 1} });



unsigned char zLeftShape1[]  = { 1, 1, 0,
                                 0, 1, 1, };
unsigned char zLeftShape2[]  = { 0, 1,
                                 1, 1,
                                 1, 0 };

Block ZLeft({ { zLeftShape1, 3, 2 }, { zLeftShape2, 2, 3 } });



unsigned char zRightShape1[] = { 0, 1, 1,
                                 1, 1, 0, };
unsigned char zRightShape2[] = { 1, 0,
                                 1, 1,
                                 0, 1 };

Block ZRight({ {zRightShape1, 3, 2}, {zRightShape2, 2, 3} });


unsigned char lLeftShape1[]  = { 1, 1, 1,
                                 0, 0, 1, };
unsigned char lLeftShape2[]  = { 0, 1,
                                 0, 1,
                                 1, 1, };
unsigned char lLeftShape3[]  = { 1, 0, 0,
                                 1, 1, 1, };
unsigned char lLeftShape4[]  = { 1, 1,
                                 1, 0,
                                 1, 0, };
Block LLeft({ {lLeftShape1, 3, 2}, {lLeftShape2, 2, 3}, {lLeftShape3, 3, 2}, {lLeftShape4, 2, 3} });



unsigned char lRightShape1[] = { 1, 1, 1,
                                 1, 0, 0, };
unsigned char lRightShape2[] = { 1, 1,
                                 0, 1,
                                 0, 1, };
unsigned char lRightShape3[] = { 0, 0, 1,
                                 1, 1, 1, };
unsigned char lRightShape4[] = { 1, 0,
                                 1, 0,
                                 1, 1, };
Block LRight({ {lRightShape1, 3, 2}, {lRightShape2, 2, 3}, {lRightShape3, 3, 2}, {lRightShape4, 2, 3} });



unsigned char tShape1[]      = { 1, 1, 1,
                                 0, 1, 0, };
unsigned char tShape2[]      = { 0, 1,
                                 1, 1,
                                 0, 1, };
unsigned char tShape3[]      = { 0, 1, 0,
                                 1, 1, 1, };
unsigned char tShape4[]      = { 1, 0,
                                 1, 1,
                                 1, 0, };

Block T({ {tShape1, 3, 2}, {tShape2, 2, 3}, {tShape3, 3, 2}, {tShape4, 2, 3} });



Block* Collection[] = { &Long, &ZLeft, &ZRight, &LLeft, &LRight, &T };
const uint32_t CollectionSize = sizeof(Collection) / sizeof(Collection[0]);

} // namespace Blocks
