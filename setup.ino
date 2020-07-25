void set_up_hardware() {
    turn_on_communication_with_computer();
    SerialUSB.println("Setting up hardware");
  
    set_up_LEDs();
    set_up_screen();
    set_up_buttons();
    set_up_buzzer();
}