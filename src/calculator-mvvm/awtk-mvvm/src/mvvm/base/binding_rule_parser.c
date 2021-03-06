﻿/**
 * File:   binding_rule.h
 * Author: AWTK Develop Team
 * Brief:  binding rule parser
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/tokenizer.h"
#include "mvvm/base/data_binding.h"
#include "mvvm/base/command_binding.h"
#include "mvvm/base/binding_rule_parser.h"

/*
 * v-on:click=
 * v-on:pointer_down=
 * v-on:key_down:ctrl_a
 * v-data:visible=
 */

static ret_t data_binding_init(data_binding_t* rule, tokenizer_t* t) {
  if (tokenizer_has_more(t)) {
    const char* prop = t->str + t->cursor;

    return object_set_prop_str(OBJECT(rule), DATA_BINDING_PROP, prop);
  }

  return RET_FAIL;
}

static ret_t command_binding_init(command_binding_t* rule, tokenizer_t* t) {
  if (tokenizer_has_more(t)) {
    const char* event = tokenizer_next(t);
    return_value_if_fail(object_set_prop_str(OBJECT(rule), COMMAND_BINDING_EVENT, event) == RET_OK,
                         RET_FAIL);

    if (tokenizer_has_more(t)) {
      const char* event_filter = tokenizer_next(t);
      return object_set_prop_str(OBJECT(rule), COMMAND_BINDING_KEY_FILTER, event_filter);
    } else {
      return RET_OK;
    }
  }

  return RET_FAIL;
}

static binding_rule_t* binding_rule_create(const char* name, bool_t inputable) {
  tokenizer_t t;
  binding_rule_t* rule = NULL;
  return_value_if_fail(tokenizer_init(&t, name, -1, ":") != NULL, NULL);

  if (tokenizer_has_more(&t)) {
    const char* type = tokenizer_next(&t);

    if (tk_str_ieq(type, BINDING_RULE_DATA_PREFIX)) {
      rule = BINDING_RULE(data_binding_create());
      if (rule != NULL) {
        if (data_binding_init((data_binding_t*)rule, &t) != RET_OK) {
          object_unref((object_t*)rule);
          rule = NULL;
        }
        object_set_prop_str(OBJECT(rule), DATA_BINDING_MODE,
                            inputable ? BINDING_STR_TWO_WAY : BINDING_STR_ONE_WAY);
      }
    } else if (tk_str_ieq(type, BINDING_RULE_COMMAND_PREFIX)) {
      rule = BINDING_RULE(command_binding_create());
      if (rule != NULL) {
        if (command_binding_init((command_binding_t*)rule, &t) != RET_OK) {
          object_unref((object_t*)rule);
          rule = NULL;
        }
      }
    }
  }
  tokenizer_deinit(&t);

  return rule;
}

binding_rule_t* binding_rule_parse(const char* name, const char* value, bool_t inputable) {
  tokenizer_t t;
  const char* k = NULL;
  const char* v = NULL;
  char key[TK_NAME_LEN + 1];
  binding_rule_t* rule = NULL;
  return_value_if_fail(name != NULL && value != NULL, NULL);

  rule = binding_rule_create(name, inputable);
  return_value_if_fail(rule != NULL, NULL);

  if (tokenizer_init_ex(&t, value, -1, " {}", "=,") == NULL) {
    object_unref(OBJECT(rule));
    return NULL;
  }

  k = tokenizer_next_expr_until(&t, ",}");
  if (k != NULL) {
    ENSURE(object_set_prop_str(OBJECT(rule), k, NULL) == RET_OK);
    while (tokenizer_has_more(&t)) {
      k = tokenizer_next(&t);
      while (k && *k == ',' && tokenizer_has_more(&t)) {
        k = tokenizer_next(&t);
      }
      tk_strncpy(key, k, TK_NAME_LEN);
      key[TK_NAME_LEN] = '\0';

      v = tokenizer_next(&t);
      if (v != NULL && *v == '=') {
        v = tokenizer_next_until(&t, ",}");
      }

      ENSURE(object_set_prop_str(OBJECT(rule), key, v) == RET_OK);
    }
  }

  tokenizer_deinit(&t);

  return rule;
}
