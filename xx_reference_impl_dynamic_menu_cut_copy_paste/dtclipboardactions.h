/**
*  License: CC-0
*  Creator: DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104
**/  

#ifndef __DT_CLIPBOARD_ACTIONS_H__
#define __DT_CLIPBOARD_ACTIONS_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TYPE_DT_CLIPBOARD_ACTIONS dt_clipboard_actions_get_type ()

G_DECLARE_INTERFACE (DtClipboardActions, dt_clipboard_actions, DT, CLIPBOARD_ACTIONS, GObject)

struct _DtClipboardActionsInterface 
{
  GTypeInterface parent;

  // -----   signals    -----
  // (notice: no prefix here)
  void (* clipboard_copy ) (DtClipboardActions *self);
  void (* clipboard_paste) (DtClipboardActions *self);
  void (* clipboard_cut  ) (DtClipboardActions *self);

  // ----- interface functions -----
  //   (notice: no prefix here)
  gboolean (*is_copy_possible)  (DtClipboardActions *self);
  gboolean (*is_cut_possible)   (DtClipboardActions *self);
  gboolean (*is_paste_possible) (DtClipboardActions *self);
};

// --------------------------
// ---- Public functions ----
// --------------------------
gboolean dt_clipboard_actions_is_copy_possible  (DtClipboardActions *self);
gboolean dt_clipboard_actions_is_cut_possible   (DtClipboardActions *self);
gboolean dt_clipboard_actions_is_paste_possible (DtClipboardActions *self);

G_END_DECLS

#endif /* include-protector */