#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define ERR(msg) ({fprintf(stderr, "\033[1;31mError:\033[0m %s!\n\r", msg); return 1;})
#define USG(msg) ({fprintf(stderr, "\033[1;31mUsage:\033[0m %s x y\n\r", msg); return 1;})

#define FBITS 32
#define SIGN_BITS 1
#define EXP_BITS 8
#define MANT_BITS 23
#define EXP_BIAS 127

#define SIGN_MASK 0x80000000u
#define EXP_MASK  0x7F800000u
#define MANT_MASK 0x007FFFFFu
#define HIDDEN_BIT 0x00800000u

static uint32_t float_to_bits(float f) {
    uint32_t x;
    memcpy(&x, &f, sizeof(x));
    return x;
}

static float bits_to_float(uint32_t x) {
    float f;
    memcpy(&f, &x, sizeof(f));
    return f;
}

static int is_nan_u(uint32_t u) { return (u & EXP_MASK) == EXP_MASK && (u & MANT_MASK) != 0; }
static int is_inf_u(uint32_t u) { return (u & EXP_MASK) == EXP_MASK && (u & MANT_MASK) == 0; }
static int is_zero_u(uint32_t u) { return (u & ~SIGN_MASK) == 0; }
//static int is_denormal_u(uint32_t u) { return (u & EXP_MASK) == 0 && (u & MANT_MASK) != 0; }

static void print_bits_uint(uint32_t v, int bits) { for (int i = bits - 1; i >= 0; --i) putchar((v & (1u << i)) ? '1' : '0'); }

static void print_float_parts(uint32_t u, const char *label, float as_decimal) {
    uint32_t sign = 0, exp = 0, mant = 0;
    
    //!TODO1
    sign = (u & SIGN_MASK) >> 31;
    exp  = (u & EXP_MASK ) >> 23;
    mant =  u & MANT_MASK;


    printf("%s = %g\n\r", label, (double)as_decimal);
    printf("Sign bit of %s: %u\n\r", label, sign);
    printf("Exponent of %s: ", label);
    print_bits_uint(exp, EXP_BITS);
    putchar('\n');
    putchar('\r');
    printf("Mantissa of %s: ", label);
    print_bits_uint(mant, MANT_BITS);
    putchar('\n');
    putchar('\n');
    putchar('\r');
}

static uint32_t ieee32_mul_u(uint32_t a, uint32_t b) {
    int32_t res_exp = 0;
    uint32_t res_sign = 0;
    uint64_t prod = 0;

    if (is_nan_u(a) || is_nan_u(b)) return 0x7FC00000;
    if (is_zero_u(a) || is_zero_u(b)) {
        if (is_inf_u(a) || is_inf_u(b)) return 0x7FC00000; // 0 * Inf
        return (a ^ b) & SIGN_MASK;
    }

    if (is_inf_u(a) || is_inf_u(b)) return ((a ^ b) & SIGN_MASK) | EXP_MASK;

    //!TODO2
    res_sign = (a ^ b) & SIGN_MASK;

    int32_t exp_a = (a & EXP_MASK) >> 23;
    int32_t exp_b = (b & EXP_MASK) >> 23;

    res_exp = exp_a + exp_b - EXP_BIAS;

    uint64_t mant_a = (a & MANT_MASK) | HIDDEN_BIT;
    uint64_t mant_b = (b & MANT_MASK) | HIDDEN_BIT;

    prod = mant_a * mant_b;

    int shift;
    if (prod & (1ULL << 47)) {
        shift = 24;
        res_exp++;
    } else {
        shift = 23;
    }

    uint64_t remainder = prod & ((1ULL << shift) - 1);
    uint64_t half = 1ULL << (shift - 1);

    prod >>= shift;

    if (remainder > half) prod++;
    else if (remainder == half && (prod & 1)) prod++;

    if (prod & (1ULL << 24)) {
        prod >>= 1;
        res_exp++;
    }

    if (res_exp >= 255) return res_sign | EXP_MASK;
    if (res_exp <= 0) return res_sign; // Underflow -> 0

    return res_sign | (res_exp << 23) | (prod & MANT_MASK);
}

static uint32_t ieee32_add_u(uint32_t a, uint32_t b) {
    int32_t res_exp = 0;
    uint32_t res_sign = 0;
    uint64_t res_mant = 0;
    
    uint32_t sign_a, sign_b;
    int32_t exp_a, exp_b;
    uint64_t mant_a, mant_b;
    uint64_t remainder, round_bits;

    if (is_nan_u(a) || is_nan_u(b)) return 0x7FC00000;
    if (is_inf_u(a) && is_inf_u(b)) return ((a & SIGN_MASK) != (b & SIGN_MASK)) ? 0x7FC00000 : a;
    if (is_inf_u(a)) return a;
    if (is_inf_u(b)) return b;
    if (is_zero_u(a)) return b;
    if (is_zero_u(b)) return a;

    //!TODO4a

    if (exp_b > exp_a || (exp_b == exp_a && mant_b > mant_a)) {
        uint32_t ts = sign_a; sign_a = sign_b; sign_b = ts;
        int32_t te = exp_a; exp_a = exp_b; exp_b = te;
        uint64_t tm = mant_a; mant_a = mant_b; mant_b = tm;
    }

    //!TODO4b
    
    res_exp = exp_a;
    res_sign = sign_a;

    if (sign_a == sign_b) {
        //!TODO4c
    } else {
        //!TODO4d

        //!TODO4e
    }

    //!TODO4f

    if (round_bits > 4) {
        res_mant++;
    } else if (round_bits == 4) {
        if (res_mant & 1) res_mant++;
    }

    //!TODO4g

    if (res_exp >= 255) return res_sign | EXP_MASK;
    if (res_exp <= 0) return res_sign;

    return res_sign | (res_exp << 23) | (res_mant & MANT_MASK);
}

uint32_t ieee32_inv_u(uint32_t x) {
    int32_t res_exp = 0;
    uint32_t res_sign = 0;
    uint64_t res_mant = 0;

    uint32_t sign;
    int32_t exp;
    uint32_t mant;
    uint64_t remainder, numerator;

     //!TODO3_Extraction

    sign = x & SIGN_MASK;
    exp  = (x & EXP_MASK) >> 23;
    mant = (x & MANT_MASK) | HIDDEN_BIT;
    
    if (is_nan_u(x)) return 0x7FC00000;             // NaN -> Quiet NaN
    if (is_zero_u(x)) return sign | EXP_MASK;       // +/- 0 -> +/- Inf
    if (is_inf_u(x)) return sign;                   // +/- Inf -> +/- 0

     //!TODO3

    res_sign = sign;
    res_exp  = 253 - exp;

    numerator = 1ULL << 47;
    res_mant = numerator / mant;
    remainder = numerator % mant;

    uint64_t half = mant >> 1;

    if (remainder > half) res_mant++;
    else if (remainder == half && (res_mant & 1)) res_mant++;

    if (res_mant & (1ULL << 24)) {
        res_mant >>= 1;
        res_exp++;
    }

    if (res_exp >= 255) return res_sign | EXP_MASK; // Overflow -> Inf
    if (res_exp <= 0)   return res_sign;            // Underflow -> 0

    return res_sign | (res_exp << 23) | ((uint32_t)res_mant & MANT_MASK);
}

static long long int ieee32_to_ll(uint32_t u) {
    int32_t exp = 0;
    uint32_t mant = 0, sign = 0;

    if (is_nan_u(u)) return 0;
    if (is_inf_u(u)) return (u & SIGN_MASK) ? 0x8000000000000000LL : 0x7FFFFFFFFFFFFFFFLL;
    if (is_zero_u(u)) return 0;

    //!TODO5a

    if (exp < 0) return 0;
    if (exp >= 63) return sign ? 0x8000000000000000LL : 0x7FFFFFFFFFFFFFFFLL;

    long long int res = mant;
    
     //!TODO5b

    return sign ? -res : res;
}

static uint32_t ll_to_ieee32(long long int i) {
    uint32_t sign = 0;
    unsigned long long uval;
    uint32_t mant = 0;
    int32_t exp = 0;
    int msb = 0, shift = 0;
    uint64_t remainder, half;

    if (i == 0) return 0;

    //!TODO6

    return sign | (exp << 23) | mant;
}

static uint32_t str_to_ieee32(char* str) {
    uint32_t res = 0;
    char *ptr = str;
    bool negative = false;
    long long frac_part = 0, frac_divisor = 1, int_part = 0;
    bool has_digits = false;
    
    if (!str || !*str) return 0x7FC00000; // NaN

    while (*ptr == ' ') ptr++;

    if (strstr(ptr, "NaN") == ptr || strstr(ptr, "nan") == ptr) return 0x7FC00000;
    if (strstr(ptr, "Inf") == ptr || strstr(ptr, "inf") == ptr) return 0x7F800000;
    if (strstr(ptr, "-Inf") == ptr || strstr(ptr, "-inf") == ptr) return 0xFF800000;

    //!TODO7

    if (negative) res |= SIGN_MASK;
    else res &= ~SIGN_MASK;

    return res;
}

static long long my_pow10(int n) {
    long long r = 1;
    for(int i=0; i<n; i++) { r *= 10; }
    return r;
}

size_t ieee32_to_str(char* str, size_t n, uint32_t val, unsigned short int pre, unsigned short int post) {
    if (n == 0) return 0;
    if (is_nan_u(val)) { snprintf(str, n, "NaN"); return strlen(str); }
    if (is_inf_u(val)) {
        if (val & SIGN_MASK) snprintf(str, n, "-Inf");
        else snprintf(str, n, "Inf");
        return strlen(str);
    }

    //!TODO8

    return strlen(str);
}

int main(int argc, char **argv) {
    float x_val = 0.0f, y_val = 0.0f;
    uint32_t x_bits = 0, y_bits = 0;
    bool use_example = 0;

    if (argc == 1) {
        use_example = true;
        x_val = -42.000f; //!TODO
        y_val = -3.1415f; //!TODO
    } else if (argc == 3) {
        char *s1 = argv[1];
        char *s2 = argv[2];
        int c1 = 0, c2 = 0;
        if (sscanf(s1, " %f %n", &x_val, &c1) != 1 || (size_t)c1 != strlen(s1)) { USG(argv[0]); }
        if (sscanf(s2, " %f %n", &y_val, &c2) != 1 || (size_t)c2 != strlen(s2)) { USG(argv[0]); }
    } else { USG(argv[0]); }

    x_bits = float_to_bits(x_val);
    y_bits = float_to_bits(y_val);
    if (use_example) { printf("Example: x = %g, y = %g\n\n\r", (double)(x_val), (double)(y_val)); }
    print_float_parts(x_bits, "x", x_val);
    print_float_parts(y_bits, "y", y_val);
    uint32_t prod_bits = ieee32_mul_u(x_bits, y_bits);
    uint32_t sum_bits  = ieee32_add_u(x_bits, y_bits);
    float prod_val = bits_to_float(prod_bits);
    float sum_val  = bits_to_float(sum_bits);
    print_float_parts(prod_bits, "x*y", prod_val);
    print_float_parts(sum_bits, "x+y", sum_val);
    uint32_t inv = ieee32_inv_u(x_bits);
    printf("inv(x) = 0x%08X\n\r1/x = %f\n\n\r", inv, bits_to_float(inv));

    float ref_sum_val = x_val + y_val;
    float ref_prod_val = x_val * y_val;
    float ref_inv_val = 1.0f / x_val;
    uint32_t ref_sum_bits = float_to_bits(ref_sum_val);
    uint32_t ref_prod_bits = float_to_bits(ref_prod_val);
    uint32_t ref_inv_bits = float_to_bits(ref_inv_val);
    if (sum_bits == ref_sum_bits) {
        printf("Addition:       \033[1;32m[OK]\033[0m (Matches hardware result 0x%08X)\n\r", sum_bits);
    } else {
        printf("Addition:       \033[1;31m[FAIL]\033[0m Expected 0x%08X, got 0x%08X\n\r", ref_sum_bits, sum_bits);
    } if (prod_bits == ref_prod_bits) {
        printf("Multiplication: \033[1;32m[OK]\033[0m (Matches hardware result 0x%08X)\n\r", prod_bits);
    } else {
        printf("Multiplication: \033[1;31m[FAIL]\033[0m Expected 0x%08X, got 0x%08X\n\r", ref_prod_bits, prod_bits);
    } if (inv == ref_inv_bits) {
        printf("Inverse:        \033[1;32m[OK]\033[0m (Matches hardware result 0x%08X)\n\r", inv);
    } else {
        printf("Inverse:        \033[1;31m[FAIL]\033[0m Expected 0x%08X, got 0x%08X\n\r", ref_inv_bits, inv);
    } printf("\n\r");

    long long int y_int = ieee32_to_ll(y_bits);
    printf("Cast Float -> LL:\n\rOriginal Float: %g\n\rResult LL:      %lld\n\r\n\r", (double)y_val, y_int);
    uint32_t y_back_bits = ll_to_ieee32(y_int);
    printf("Cast LL -> Float:\n\rOriginal LL:    %lld\n\r", y_int);
    print_float_parts(y_back_bits, "Result Float", bits_to_float(y_back_bits));

    char buffer[64];
    ieee32_to_str(buffer, sizeof(buffer), y_bits, 3, 5);
    printf("String Check -> Str:\n\rResult Str:     '%s'\n\r", buffer);
    uint32_t parsed_bits = str_to_ieee32(buffer);
    printf("String Check -> Float:\n\rParsed Float:   %g\n\r", (double)bits_to_float(parsed_bits));
    if (parsed_bits == float_to_bits(atof(buffer))) { 
         printf("Verification:   \033[1;32m[OK]\033[0m Re-parsed matches atof()\n\r");
    } else {
         printf("Verification:   Check (Parsed 0x%08X vs Original 0x%08X)\n\r", parsed_bits, y_bits);
    }

    if (use_example) { USG(argv[0]); }
    return 0;
}