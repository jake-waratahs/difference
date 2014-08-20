#include <pebble.h>

//Function declarations
static void main_window_load (Window *window);
static void main_window_unload(Window *window);
static void init(void);
static void deinit(void);
void app_event_loop(void);
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
static void update_time(void);
  
  
//Global pointer to window object and textlayer object
static Window *s_main_window;
static TextLayer *s_text_layer;
static TextLayer *s_time_layer;
//static TextLayer *s_date_layer;


//Main function
int main(void){
  init();
  app_event_loop();
  deinit();  
}


  
static void init (void){
  //allocate memory for the window
  s_main_window = window_create();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
   
  //Not 100% sure on what this does
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
   
 
  //I think this pushes onto a stack
  //It actually gets the watch to display the window
  window_stack_push(s_main_window, true);

  update_time();
}

static void deinit(void){
  tick_timer_service_unsubscribe();
   //free()
  window_destroy(s_main_window);
}

static void main_window_load(Window *window) {
  
  //Create the text layer (allocate memory) for the "It's time to make a difference"
  s_text_layer = text_layer_create(GRect(0,0,144,100));
  
  //Make it purdy
  text_layer_set_background_color(s_text_layer, GColorBlack);
  text_layer_set_text_color(s_text_layer, GColorClear);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text(s_text_layer, "It's time to make a difference");
  
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  
  //Make it a child layer of the window layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer));

  //Now do the time layer

  s_time_layer = text_layer_create(GRect(0,100,144,68));
  
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorClear);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
   
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
   
  //Now the date layer
   /*
  s_date_layer = text_layer_create(GRect(0,129,144,39));
  
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_date_layer, GAlignBottom);
   
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));  
 */
}


static void main_window_unload(Window *window) {
  //free()
  text_layer_destroy(s_text_layer);
  text_layer_destroy(s_time_layer);
  //text_layer_destroy(s_date_layer);
}

static void update_time(void){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char time_buffer[] = "00:00";
 
  if (clock_is_24h_style() == true){
     strftime(time_buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
     strftime(time_buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  if ((clock_is_24h_style() == false) && (time_buffer[0] == '0')){
     text_layer_set_text(s_time_layer, &time_buffer[1]);
  } else{
     text_layer_set_text(s_time_layer, time_buffer); 
  }
   
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
   update_time();
}