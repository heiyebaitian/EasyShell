void eshell_initializer(){
    SERIAL.begin(115200);
    SERIAL.println("");
    SERIAL.println("****************************************");
    SERIAL.println("*                                      *");
    SERIAL.println("*          EASYSHELL v0.1.3            *");
    SERIAL.println("*                                      *");
    SERIAL.println("****************************************");
}
