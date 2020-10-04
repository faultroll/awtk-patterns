﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "temperature_view_model.h"

static ret_t temperature_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  if (tk_str_ieq("temp", name)) {
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t temperature_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  if (tk_str_ieq("temp", name)) {
    value_set_double(v, 123);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static bool_t temperature_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  if (tk_str_ieq("apply_temp", name)) {
    return TRUE;
  }
  return FALSE;
}

static ret_t temperature_view_model_exec(object_t* obj, const char* name, const char* args) {
  if (tk_str_ieq("apply_temp", name)) {
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t temperature_view_model_on_destroy(object_t* obj) {
  temperature_view_model_t* vm = (temperature_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_temperature_view_model_vtable = {"temperature_view_model_t",
                                                                "temperature_view_model_t",
                                                                sizeof(temperature_view_model_t),
                                                                FALSE,
                                                                temperature_view_model_on_destroy,
                                                                NULL,
                                                                temperature_view_model_get_prop,
                                                                temperature_view_model_set_prop,
                                                                NULL,
                                                                NULL,
                                                                temperature_view_model_can_exec,
                                                                temperature_view_model_exec};

view_model_t* temperature_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_temperature_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  temperature_view_model_t* temperature_view_model = (temperature_view_model_t*)(vm);

  return vm;
}
