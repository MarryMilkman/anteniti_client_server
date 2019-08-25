#ifndef ENCRYPTOR_HPP
# define ENCRYPTOR_HPP

# include "lib.h"

#include <random>

class Encryptor {
public:
    Encryptor();
    ~Encryptor();

    char                *encrypt(std::string mess);
    static std::string  decrypt(std::string mess);
private:
    std::string         _key;
    std::string         _encrypted_text;

    void                _randomize_key();
    void                _encrypt_text(std::string mess);
    char                *_get_final_buff();

    static char         _encrypd_symbol(char symbol, char key);
    static char         _decrypd_symbol(unsigned char symbol, char key);

};

#endif
