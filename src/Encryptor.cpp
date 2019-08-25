#include "Encryptor.hpp"

Encryptor::Encryptor() {
}

Encryptor::~Encryptor() {

}


    // MARK : Encrypt message

char    *Encryptor::encrypt(std::string mess) {
    this->_randomize_key();
    this->_encrypt_text(mess);
    return this->_get_final_buff();
}

void    Encryptor::_randomize_key() {
    std::random_device                  rd;
    std::mt19937                        e2(rd());
    std::uniform_int_distribution<int>  dist(100000, 999999);
    unsigned int                        i = 0;
    std::string                         s1;
    std::string                         s2;

    while (++i <= 16) {
        if (i <= 8)
            s1 += std::to_string(dist(e2));
        else
            s2 += std::to_string(dist(e2));
    }

    int     size = s1.size();
    char    buff[size + 1];

    i = -1;
    while (++i < s1.size())
        buff[i] = static_cast<unsigned char>((s1[i] - 48) * 10 + (s2[i] - 48) + 27);
    buff[i] = 0;
    this->_key = buff;
}

void    Encryptor::_encrypt_text(std::string mess) {
    int         i = 0;
    int         j = 0;
    int         size_mess = mess.size();
    int         size_key = this->_key.size();
    char        buff[size_mess + 1];

    while (i < size_mess) {
        if (j >= size_key)
            j = 0;
        buff[i] = Encryptor::_encrypd_symbol(mess[i], this->_key[j++]);;
        i++;
    }
    buff[i] = 0;
    this->_encrypted_text = buff;
}

char    *Encryptor::_get_final_buff() {
    char            *final_buff;
    unsigned int    i;
    int             j;
    unsigned int    size = this->_encrypted_text.size() + this->_key.size();

    final_buff = (char *)malloc(size + 1);
    i = 0;
    while (i < this->_encrypted_text.size()) {
        final_buff[i] = this->_encrypted_text[i];
        i++;
    }
    j = 0;
    while (i < size)
        final_buff[i++] = this->_key[j++];
    final_buff[i] = 0;
    return final_buff;
}


    // MARK : decrypt message

std::string     Encryptor::decrypt(std::string mess) {
    std::string     text;
    int             i = 0;
    int             j = 0;
    int             size_mess = mess.size() - 48;
    int             size_key = 48;
    char            key[size_key];
    char            buff[size_mess];

    while (i < 48) {
        key[i] = mess[i + size_mess];
        i++;
    }
    i = 0;
    while (i < size_mess) {
        if (j == size_key)
            j = 0;
        buff[i] = Encryptor::_decrypd_symbol(mess[i], key[j++]);
        i++;
    }
    buff[i] = 0;
    text = buff;
    return text;
}


    // MARK : work with symbol

char            Encryptor::_encrypd_symbol(char symbol, char key) {
    // if (symbol > key / 2 && symbol - key / 2 < key / 2)
    //     return symbol - key / 2;
    return (symbol + key);
}

char            Encryptor::_decrypd_symbol(unsigned char symbol, char key) {
    // if (symbol < key / 2)
    //     return symbol + key / 2;
    return (symbol - key);
}
