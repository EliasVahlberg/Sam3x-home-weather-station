/**
* @file
*     LoginSystem.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/
#pragma region functions
void login_init();
void login();
int login_get_user_input(char *dest, screen_element *sc_el);
int create_user();
int edit_user();
void hash_password(char *password, int length);
#pragma endregion functions

void login_init()
{
    users = malloc(sizeof(user_list));
    if (users == NULL)
        return;
    default_user = malloc(sizeof(user));
    if (default_user == NULL)
        return;
    (*default_user).u_len = def_us_len;
    (*default_user).p_len = def_pass_len;
    (*default_user).username = (char *)malloc(def_us_len * sizeof(char));
    if ((*default_user).username == NULL)
        return;
    (*default_user).password = (char *)malloc(def_pass_len * sizeof(char));
    if ((*default_user).password == NULL)
        return;
    for (int i = 0; i < def_us_len; i++)
    {
        (*default_user).username[i] = default_username[i];
    }
    for (int i = 0; i < def_pass_len; i++)
    {
        (*default_user).password[i] = default_password[i];
    }
    (*default_user).acess_level = 0;
    users->this_user = default_user;
    users->next_user = NULL;
}

void screen_pre_login(char *head_text, int head_text_length)
{
    clear();
    pre_login       = malloc(sizeof(screen_element));
    if (pre_login == NULL)
        return;
    pre_username    = malloc(sizeof(screen_element));
    if (pre_username == NULL)
        return;
    pre_password    = malloc(sizeof(screen_element));
    if (pre_password == NULL)
        return;
    screen_username = malloc(sizeof(screen_element));
    if (screen_username == NULL)
        return;
    screen_password = malloc(sizeof(screen_element));
    if (screen_password == NULL)
        return;
    (*pre_login) = create_screen_element(0, 0, head_text_length, head_text);
    (*pre_username) = create_screen_element(LOGIN_USERNAME_X_POS, LOGIN_USERNAME_Y_POS, usr_pass_str_length, login_user_str);
    (*pre_password) = create_screen_element(LOGIN_PASSWORD_X_POS, LOGIN_PASSWORD_Y_POS, usr_pass_str_length, login_pass_str);
    display_write(*pre_login);
    display_write(*pre_username);
    display_write(*pre_password);
}

void screen_post_login()
{
    char_array_erase((*screen_username).text, (*screen_username).l, (char)0); //For security reasons
    char_array_erase((*screen_password).text, (*screen_password).l, (char)0); //For security reasons
    free(screen_username);
    free(screen_password);
    free(pre_login);
    free(pre_username);
    free(pre_password);
    clear();
}

void login()
{
    screen_pre_login(login_text, login_text_length);
    while (1)
    {
        char screen_usrname[LOGIN_USERNAME_MAX_LEN];
        char screen_pass[LOGIN_PASSWORD_MAX_LEN];
        char username[56];
        char password[56];
        int d = 0, e = 0;
        (*screen_username) = create_screen_element(LOGIN_USERNAME_X_POS + usr_pass_str_length, LOGIN_USERNAME_Y_POS, d, screen_usrname);
        (*screen_password) = create_screen_element(LOGIN_PASSWORD_X_POS + usr_pass_str_length, LOGIN_PASSWORD_Y_POS, e, screen_pass);
        int username_length = login_get_user_input(username, screen_username);
        int password_length = login_get_user_input(password, screen_password);
        hash_password(password, password_length);
        if (validate_login(username, username_length, password, password_length) == 1)
        {
            display_clear((*screen_username).l, (*screen_username).screen_cord.pos, (*screen_username).screen_cord.screen_half_val);
            display_clear((*screen_password).l, (*screen_password).screen_cord.pos, (*screen_password).screen_cord.screen_half_val);

            char_array_erase((*screen_username).text, (*screen_username).l, (char)0); //For security reasons
            char_array_erase((*screen_password).text, (*screen_password).l, (char)0); //For security reasons
            break;
        }
    }
    screen_post_login();
}

int login_get_user_input(char *dest, screen_element *sc_el)
{
    unsigned char c;
    int length = 0;
    char zeroSet = 0;
    while (length != 56) //max length of username
    {
        delay_milis(50);
        c = (unsigned char)(get_keypad_key());
        if (c == 0)
        {
            zeroSet = 1;
            continue;
        }
        if (zeroSet == 1)
        {
            if ((int)c == 12) //When pressing "#" set username
            {
                break;
            }
            else if ((int)c == 10) //when pressing "*" delete prev char
            {
                dest[(length != 0) ? --length : length] = 0;
                (*sc_el).text[length] = (char)0;
                display_write(*sc_el);
                (*sc_el).l = ((*sc_el).l != 0) ? (*sc_el).l - 1 : 0;
                zeroSet = 0;
                continue;
            }
            else if ((int)c == 11) //When pressing "0" set c to 48 i.e. 0
            {
                c = 0;
            }

            c += 48;
            (*sc_el).text[length] = c;
            (*sc_el).l = ((*sc_el).l != LOGIN_USERNAME_MAX_LEN) ? (*sc_el).l + 1 : (*sc_el).l;
            dest[length++] = c;
            display_write(*sc_el);
            zeroSet = 0;
        }
    }

    return length;
}

int validate_login(char *new_username, int new_username_len, char *new_password, int new_password_len)
{
    user_list *temporary = users;
    while (temporary->this_user != NULL)
    {
        if (new_username_len == (*temporary).this_user->u_len)
        {
            char same_username = 1;
            for (int i = 0; i < new_username_len; i++)
            {
                if ((*temporary).this_user->username[i] != new_username[i])
                {
                    same_username = 0;
                    break;
                }
            }
            if (same_username == 1)
            {
                int same_password = 1;
                for (int i = 0; i < new_password_len; i++)
                {
                    if ((*temporary).this_user->password[i] != new_password[i])
                    {
                        same_password = 0;
                        break;
                    }
                }
                if (same_password == 1)
                {
                    current_user = (*temporary).this_user;
                    return 1;
                }
            }
        }
        if (temporary->next_user == NULL)
            break;
        temporary = temporary->next_user;
    }
    return 0;
}

int create_user()
{
}

int edit_user()
{
    if (current_user != NULL)
    {
        screen_pre_login(edit_user_text, edit_user_text_length);

        char screen_usrname[LOGIN_USERNAME_MAX_LEN];
        char screen_pass[LOGIN_PASSWORD_MAX_LEN];
        int d = 0, e = 0;
        (*screen_username) = create_screen_element(LOGIN_USERNAME_X_POS + usr_pass_str_length, LOGIN_USERNAME_Y_POS, d, screen_usrname);
        (*screen_password) = create_screen_element(LOGIN_PASSWORD_X_POS + usr_pass_str_length, LOGIN_PASSWORD_Y_POS, e, screen_pass);
        char new_username[56];
        char new_password[56];
        int username_len = login_get_user_input(new_username, screen_username);
        int pass_len = login_get_user_input(new_username, screen_password);
        if (username_len != 0)
        {
            (*current_user).u_len = username_len;
            (*current_user).username = new_username;
        }
        if (pass_len != 0)
        {
            hash_password(new_password, pass_len);
            (*current_user).p_len = pass_len;
            (*current_user).password = new_password;
        }

        screen_post_login();
        return 1;
    }
    return 0;
}

void hash_password(char *password, int length)
{
}
