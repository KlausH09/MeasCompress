#ifndef CPP_PYMATHMODULE_HPP
#define CPP_PYMATHMODULE_HPP

/**
 * @brief Add two integers
 * 
 * @param i an integer
 * @param j another integer
 * @return int 
 */
int add(int i, int j)
{
    return i + j;
}

/**
 * @brief Subtract one integer from another 
 * 
 * @param i an integer
 * @param j an integer to subtract from \p i
 * @return int 
 */
int subtract(int i, int j)
{
    return i - j;
}

#endif
