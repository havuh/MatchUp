#ifndef FRIEND_H
#define FRIEND_H


//PENDIENT      = ALGUIEN LO QUIERE AGREGAR A SUS AMIGOS,
//NO_CONFIRMED  = ENVIO SOLICITUD PENDIENTE DE CONFIRMAR,
//CONFIRMED     = SON AMIGOS

class Friend
{
public:
    enum Condition { no_confirmed, pendient, confirmed };

private:
    unsigned int	code;
    Condition		is_add;

public:
    Friend() {}
    Friend(unsigned int code, Condition is_add = no_confirmed) : code(code), is_add(is_add) {}
    ~Friend() {}

    void			setCode(unsigned int codeUser) { this->code = codeUser  ; }
    void			setIsAdd(Condition is_add) { this->is_add = is_add      ; }

    unsigned int	getCode() { return code     ; }
    int				getIsAdd() { return is_add  ; }
};

#endif // FRIEND_H
