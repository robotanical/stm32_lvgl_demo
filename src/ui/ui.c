#include "inc/ui/ui.h"
void draw_event_cb(lv_event_t * e)
{
    lv_draw_task_t * draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base_dsc = draw_task->draw_dsc;
    if(base_dsc->part == LV_PART_INDICATOR) {
        lv_obj_t * obj = lv_event_get_target(e);
        lv_chart_series_t * ser = lv_chart_get_series_next(obj, NULL);
        lv_draw_rect_dsc_t * rect_draw_dsc = draw_task->draw_dsc;
        uint32_t cnt = lv_chart_get_point_count(obj);

        /*Make older value more transparent*/
        rect_draw_dsc->bg_opa = (LV_OPA_COVER *  base_dsc->id2) / (cnt - 1);

        /*Make smaller values blue, higher values red*/
        int32_t * x_array = lv_chart_get_x_array(obj, ser);
        int32_t * y_array = lv_chart_get_y_array(obj, ser);
        /*dsc->id is the tells drawing order, but we need the ID of the point being drawn.*/
        uint32_t start_point = lv_chart_get_x_start_point(obj, ser);
        uint32_t p_act = (start_point + base_dsc->id2) % cnt; /*Consider start point to get the index of the array*/
        lv_opa_t x_opa = (x_array[p_act] * LV_OPA_50) / 200;
        lv_opa_t y_opa = (y_array[p_act] * LV_OPA_50) / 1000;

        rect_draw_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_RED),
                                               lv_palette_main(LV_PALETTE_BLUE),
                                               x_opa + y_opa);
    }
}

void add_data(lv_timer_t * timer){
	static int i;
    LV_UNUSED(timer);
    lv_obj_t * chart = timer->user_data;
    lv_chart_set_next_value2(chart, lv_chart_get_series_next(chart, NULL), i++, 25+lv_rand(0, 5));
    if (i>1000)
    {
    	lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, i-1000,
			 i);
    }
};

void init_main_screen(){
		lv_obj_t *screen;
		lv_obj_t *text_label;
		lv_obj_t *humidity_label;
		lv_style_t style;
		lv_obj_t *grid_cont;


		static int32_t col_dsc[] = {200, 280, LV_GRID_TEMPLATE_LAST};
		static int32_t row_dsc[] = {136, 136, LV_GRID_TEMPLATE_LAST};
		LV_FONT_DECLARE(muli_light_24);
	    lv_obj_t * cont = lv_obj_create(lv_scr_act());
	    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
	    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
	    lv_obj_set_size(cont, 480, 272);
	    lv_obj_center(cont);
	    lv_obj_set_layout(cont, LV_LAYOUT_GRID);
	    lv_obj_set_style_pad_all(cont, 0, 0);
	    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
	    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
	    lv_obj_set_style_pad_column(cont, 0, 0);
	    lv_obj_set_style_pad_row(cont, 0, 0);
	    lv_obj_set_style_border_width(cont, 0, 0);


		lv_obj_t *text_label_cont = lv_obj_create(cont);
		lv_obj_set_size(text_label_cont, 200, 136);
		lv_obj_set_grid_cell(text_label_cont, LV_GRID_ALIGN_START, 0, 1,
					 LV_GRID_ALIGN_START, 0, 1);
		text_label = lv_label_create(text_label_cont);
	    lv_obj_set_style_border_width(text_label_cont, 0, 0);
		lv_obj_set_style_text_font(text_label, &muli_light_24, 0);
		lv_label_set_long_mode(text_label, LV_LABEL_LONG_WRAP);
		lv_label_set_text_fmt(text_label,
						   "Temperatura\n" "dobowa:");

		lv_obj_t* humidity_label_cont;
		humidity_label_cont = lv_obj_create(cont);
		lv_obj_set_size(humidity_label_cont, 200, 136);
		lv_obj_set_grid_cell(humidity_label_cont, LV_GRID_ALIGN_START, 0, 1,
						  LV_GRID_ALIGN_CENTER, 1, 1);
	    lv_obj_set_style_border_width(humidity_label_cont, 0, 0);
	    humidity_label= lv_label_create(humidity_label_cont);
		lv_obj_set_style_text_font(humidity_label, &muli_light_24, 0);
		lv_label_set_long_mode(humidity_label, LV_LABEL_LONG_WRAP);
		lv_label_set_text_fmt(humidity_label,
						   "Wilgotność\n" "powietrza:");

		/*Create a chart*/
		lv_obj_t * chart_cont = lv_obj_create(cont);
		lv_obj_set_size(chart_cont, 280, 136);
//		lv_obj_set_style_pad_top(chart_cont, 20, 0);
		lv_obj_set_style_pad_left(chart_cont, 20, 0);
	    lv_obj_set_scrollbar_mode(chart_cont, LV_SCROLLBAR_MODE_OFF);



		lv_obj_set_grid_cell(chart_cont, LV_GRID_ALIGN_CENTER, 1, 1,
			LV_GRID_ALIGN_CENTER, 0, 1);
		lv_obj_t * chart;
		chart = lv_chart_create(chart_cont);
		lv_obj_set_style_border_width(chart_cont, 0, 0);
		lv_obj_set_size(chart, 200, 106);

//		lv_obj_add_event_cb(chart, draw_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
//		lv_obj_add_flag(chart, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);
		lv_obj_set_style_line_width(chart, 1, LV_PART_ITEMS);   /*Remove the lines*/
		lv_obj_set_style_width(chart, 1, LV_PART_INDICATOR);   /*Remove the lines*/
		lv_chart_set_type(chart, LV_CHART_TYPE_SCATTER);   /*Show lines and points too*/
		lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0,
							 1000);
		lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 60);
		lv_chart_set_point_count(chart, 1000);
		lv_obj_set_style_pad_right(chart, 0, LV_PART_MAIN);
		lv_obj_set_style_pad_left(chart, 0, LV_PART_MAIN);
		lv_obj_set_style_border_width(chart, 0, 0);  // border
		lv_obj_t* y_axis_scale = lv_scale_create(chart_cont);
		lv_scale_set_mode(y_axis_scale, LV_SCALE_MODE_VERTICAL_LEFT);
		lv_obj_set_size(y_axis_scale,lv_pct(0),106);
	    lv_scale_set_total_tick_count(y_axis_scale, 6);
	    lv_scale_set_major_tick_every(y_axis_scale, 1);
	    lv_scale_set_range(y_axis_scale, 0, 60);
	    lv_obj_set_style_pad_hor(y_axis_scale, lv_chart_get_first_point_center_offset(chart), 0);

		lv_chart_series_t * ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);//		    uint32_t i;

		lv_timer_create(add_data, 10, chart);

};
