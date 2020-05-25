/**
*  License: CC-0
*  Creator: DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104
**/  

#include "dtclipboardactions.h"

// ===================================================================

// define convenience macros:

#ifndef G_DISABLE_CHECKS
// -------------------------------
// @brief: if 'expr' != TRUE, print critical message.
#ifndef gx_critical_if_fail
  #define gx_critical_if_fail(expr)                       \
  G_STMT_START {                                          \
    if (! G_LIKELY (expr))                                \
      {                                                   \
        g_critical( "%s (%s:%d): assertion '%s' failed.", \
                  G_STRFUNC, __FILE__, __LINE__ , #expr); \
      }                                                   \
  } G_STMT_END  

// -------------------------------
// @brief: if 'expr' != TRUE, print critical message. The message
//         contains the G_OBJECT_TYPE_NAME of the object.
#define gx_critical_if_fail_object(expr, object)                     \
  G_STMT_START {                                                     \
    if (! G_LIKELY (expr))                                           \
      {                                                              \
        g_critical( "%s (%s:%d): assertion '%s' failed on type %s.", \
                  G_STRFUNC, __FILE__, __LINE__ ,                    \
                  #expr, G_OBJECT_TYPE_NAME(object) );               \
      }                                                              \
  } G_STMT_END
#endif  
#endif
// -------------------------------

// define GLib eXtended functions
#ifndef GX_FUNCTIONS
  #define GX_FUNCTIONS

  /** @brief: Convenience function. Creates NULL checks and calls the functions
            every extended check should be performed before calling the macro.
            First parameter of the function must be called "self"
      @example: 
                void
                my_new_iface_testfunction ( MyNewIface *self,
                                            gint        value)
                {
                  
                  GX_DEFINE_BASIC_IFACE_FUNCTION(MyNewIface,MY,NEW_IFACE, testfunction, (self, value));
                }
  */
  #define GX_DEFINE_BASIC_IFACE_FUNCTION(class_camelcase, moduleUpper, classUpper, vfunction, function_params) \
            gx_critical_if_fail_object ( moduleUpper##_IS_##classUpper (self), self);                    \
            class_camelcase##Interface * object = moduleUpper##_##classUpper##_GET_IFACE (self);         \
            gx_critical_if_fail_object (NULL != object-> vfunction, self);                          \
            return object->vfunction function_params;
  
#endif

// ===================================================================

G_DEFINE_INTERFACE (DtClipboardActions, dt_clipboard_actions, G_TYPE_OBJECT)

// ===================================================================

// define signal indices
enum {
  SIGNAL_COPY,
  SIGNAL_CUT,
  SIGNAL_PASTE,
  N_SIGNALS
};

static guint signals[N_SIGNALS];

// ===================================================================


// -------------------------------------------------------------------

gboolean
dt_clipboard_actions_is_copy_possible(DtClipboardActions *self)
{
  GX_DEFINE_BASIC_IFACE_FUNCTION(DtClipboardActions, DT, CLIPBOARD_ACTIONS , is_copy_possible, (self));
}
// -------------------------------------------------------------------

gboolean
dt_clipboard_actions_is_cut_possible(DtClipboardActions *self)
{
  GX_DEFINE_BASIC_IFACE_FUNCTION(DtClipboardActions, DT, CLIPBOARD_ACTIONS , is_cut_possible, (self));
}
// -------------------------------------------------------------------

gboolean
dt_clipboard_actions_is_paste_possible(DtClipboardActions *self)
{
  GX_DEFINE_BASIC_IFACE_FUNCTION(DtClipboardActions, DT, CLIPBOARD_ACTIONS , is_paste_possible, (self));
}

// -------------------------------------------------------------------


// ===================================================================

static void
dt_clipboard_actions_default_init(DtClipboardActionsInterface * klass)
{
    // if wanted: setup default implementation for methods

    // setup signals
    signals[SIGNAL_COPY] = g_signal_new ("copy-clipboard", 
                                TYPE_DT_CLIPBOARD_ACTIONS, G_SIGNAL_RUN_LAST,
                                G_STRUCT_OFFSET (DtClipboardActionsInterface, clipboard_copy),
                                0, NULL, NULL, G_TYPE_NONE, 0);
    signals[SIGNAL_CUT] = g_signal_new ("cut-clipboard", 
                                TYPE_DT_CLIPBOARD_ACTIONS, G_SIGNAL_RUN_LAST,
                                G_STRUCT_OFFSET (DtClipboardActionsInterface, clipboard_cut),
                                0, NULL, NULL, G_TYPE_NONE, 0);
    signals[SIGNAL_PASTE] = g_signal_new ("paste-clipboard", 
                                TYPE_DT_CLIPBOARD_ACTIONS, G_SIGNAL_RUN_LAST,
                                G_STRUCT_OFFSET (DtClipboardActionsInterface, clipboard_paste),
                                0, NULL, NULL, G_TYPE_NONE, 0);

}

// ===================================================================

