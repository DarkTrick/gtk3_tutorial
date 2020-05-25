<!--
 License: CC-BY
 Creator: DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104
--> 

The custom_widget_generator can create a custom widget or a custom interface for you.


Interface Mini Tutorial
=======================
An Interface is basically the same as a custom widget, but without the
possibility of instantiation. 

Implementation of interface methods
-----------------------------------
The interface "class" looks like this (pseudo code):

    // There's a function pointer inside the interface
    struct _MyInterface {
        void (*myfunction)(MyInterface*) = NULL;
    }
    
    // There is a public method, which will just call the 
    //  function stored inside the pointer
    void my_interface_my_function ( MyInterface * self)
    {
      myfunction (self);
    }
    
The real class implementing the interface looks like this

    // Define the implementation of the function
    void my_class_my_function (MyClass * self)
    {
      printf ("hello\n");
    }

    // Set the function pointer to the defined function    
    my_class_init_my_interface (MyInterface *klass)
    {
      myfunction = my_class_my_function;
    }
      
    
How to implement signals?
-------------------------
Defining signals is pretty much like defining functions. You just need the addition of "registering" them.

Here's a rough outline of the code with a signal:

    // create a `my_testsignal` function pointer 
    struct _MyInterface {
      void (*my_testsignal)( /*random params*/ );
    }
    
    enum {
      SIGNAL_TESTSIGNAL,
      N_SIGNALS
    };
    static gint signals[N_SIGNALS];
    
    // register the signal. 
    my_interface_default_init ( klass )
    {
      signals[SIGNAL_TESTSIGNAL] =
                 g_new_signal ("my-testsignal", // signal name goes here
                                TYPE_MY_INTERFACE, G_SIGNAL_RUN_LAST,
                                G_STRUCT_OFFSET (MyInterfaceInterface, my_testsignal),
                                0, NULL, NULL, 
                                G_TYPE_NONE,    // return type `void`
                                0               // no parameters
                                );
    }
    
Everything else is exactly like with function definitions. Just overwrite the 
`my_testsignal` function pointer within `my_class_init_my_interface` function 
as shown above.
