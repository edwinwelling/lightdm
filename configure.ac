dnl Process this file with autoconf to produce a configure script.

AC_INIT(lightdm, 1.32.0)
AC_CONFIG_MACRO_DIR(m4)
AC_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([1.11 no-dist-gzip dist-xz foreign])
AM_SILENT_RULES(yes)
AC_PROG_CC_C99
LT_INIT
AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MKDIR_P
AM_MAINTAINER_MODE

dnl Check if the compiler really supports `-std=c99`.
if test "x$ac_cv_prog_cc_c99" = xno; then
    AC_MSG_ERROR([$CC does not support '-std=c99'.])
fi

dnl Enable compile warnings (only supporting GCC)
if test "x$GCC" = xyes; then
    WARN_CFLAGS="-Wall \
                 -Wstrict-prototypes \
                 -Wnested-externs \
                 -Werror=missing-prototypes \
                 -Werror=implicit-function-declaration \
                 -Werror=pointer-arith \
                 -Werror=init-self \
                 -Werror=format-security \
                 -Werror=format=2 \
                 -Werror=missing-include-dirs"
    WARN_CXXFLAGS="-Wall"
fi
AC_SUBST(WARN_CFLAGS)
AC_SUBST(WARN_CXXFLAGS)

GOBJECT_INTROSPECTION_CHECK(0.9.5)

VAPIGEN_CHECK([0.16])

dnl ###########################################################################
dnl Dependencies
dnl ###########################################################################

AC_CHECK_HEADERS(security/pam_appl.h, [], AC_MSG_ERROR(PAM not found))

AC_CHECK_HEADERS(gcrypt.h, [], AC_MSG_ERROR(libgcrypt not found))

AC_CHECK_FUNCS(setresgid setresuid clearenv __getgroups_chk)

PKG_CHECK_MODULES(LIGHTDM, [
    glib-2.0 >= 2.44
    gio-2.0 >= 2.26
    gio-unix-2.0
    xdmcp
    xcb
])

PKG_CHECK_MODULES(GLIB, [
    glib-2.0
])

PKG_CHECK_MODULES(GIO, [
    gio-2.0
])

PKG_CHECK_MODULES(GIO_UNIX, [
    gio-unix-2.0
])

PKG_CHECK_MODULES(GOBJECT, [
    gobject-2.0
])

PKG_CHECK_MODULES(XCB, [
    xcb
])    

PKG_CHECK_MODULES(LIBLIGHTDM_GOBJECT, [
    glib-2.0 >= 2.44
    gio-2.0 >= 2.26
    gio-unix-2.0
    gobject-2.0
    libxklavier
    x11
])

AC_ARG_ENABLE(liblightdm-qt5,
	AS_HELP_STRING([--enable-liblightdm-qt5],[Enable LightDM client Qt5 libraries [[default=auto]]]),
	[enable_liblightdm_qt5=$enableval],
	[enable_liblightdm_qt5="auto"])
compile_liblightdm_qt5=no
if test x"$enable_liblightdm_qt5" != "xno"; then
    PKG_CHECK_MODULES(LIBLIGHTDM_QT5, [
        Qt5Core
        Qt5DBus
        Qt5Gui
    ],
    [compile_liblightdm_qt5=yes],
    [if test "x$enable_liblightdm_qt5" != xauto; then
        AC_MSG_FAILURE(
          [--enable-liblightdm-qt5 was given, but test for Qt5 failed])
      fi
    ])

    AC_CHECK_TOOLS(MOC5, [moc-qt5 moc])
    if test x$MOC5 = x; then
        compile_liblightdm_qt5=no
        if test "x$enable_liblightdm_qt5" != xauto; then
            AC_MSG_FAILURE(
              [--enable-liblightdm-qt5 was given, but MOC not found])
        fi
    fi
    if test x"$MOC5" = xmoc; then
        if test x"$(readlink $(which $MOC5))" = xqtchooser; then
            MOC5="$MOC5 --qt=5"
        fi
    fi
fi
AM_CONDITIONAL(COMPILE_LIBLIGHTDM_QT5, test x"$compile_liblightdm_qt5" != "xno")

AC_ARG_ENABLE([libaudit],
    AS_HELP_STRING([--enable-libaudit],
                   [Enable libaudit logging of login and logout events [[default=auto]]]),
    [enable_libaudit=$enableval],
    [enable_libaudit=auto])
use_libaudit=no    
if test x"$enable_libaudit" != "xno"; then
    AC_CHECK_LIB([audit], [audit_log_user_message],
                 [use_libaudit=yes
                  AC_DEFINE(HAVE_LIBAUDIT, 1, [libaudit support])
                  LIGHTDM_LIBS="${LIGHTDM_LIBS} -laudit"
                 ],
                 [if test "x$enable_libaudit" != xauto; then
                    AC_MSG_FAILURE(
                      [--enable-libaudit was given, but test for libaudit failed])
                  fi
                 ])
fi

AC_MSG_CHECKING(whether to build tests)
AC_ARG_ENABLE(tests,
        AS_HELP_STRING([--disable-tests], [Disable tests building]),
        [], [enable_tests="yes"])
AM_CONDITIONAL(COMPILE_TESTS, test x"$enable_tests" != "xno")

dnl ###########################################################################
dnl Configurable values
dnl ###########################################################################

DEFAULT_USER_SESSION=default
AC_ARG_WITH(user-session,
            AS_HELP_STRING(--with-user-session=<name>,
                           Session to use for user accounts),
    if test x$withval != x; then
        DEFAULT_USER_SESSION="$withval"
    fi
)
AC_SUBST(DEFAULT_USER_SESSION)
AC_DEFINE_UNQUOTED(DEFAULT_USER_SESSION, "$DEFAULT_USER_SESSION", User session)

DEFAULT_GREETER_SESSION=default
AC_ARG_WITH(greeter-session,
            AS_HELP_STRING(--with-greeter-session=<session>,
                           Greeter session),
    if test x$withval != x; then
        DEFAULT_GREETER_SESSION="$withval"
    fi
)
AC_SUBST(DEFAULT_GREETER_SESSION)
AC_DEFINE_UNQUOTED(DEFAULT_GREETER_SESSION, "$DEFAULT_GREETER_SESSION", Greeter session)

GREETER_USER=lightdm
AC_ARG_WITH(greeter-user,
            AS_HELP_STRING(--with-greeter-user=<username>,
                           User to run greeter as),
    if test x$withval != x; then
        GREETER_USER="$withval"
    fi
)
AC_SUBST(GREETER_USER)
AC_DEFINE_UNQUOTED(GREETER_USER, "$GREETER_USER", User to run greeter as)

dnl ###########################################################################
dnl Documentation
dnl ###########################################################################

GTK_DOC_CHECK
YELP_HELP_INIT

dnl ###########################################################################
dnl Internationalization
dnl ###########################################################################

IT_PROG_INTLTOOL(0.35.0)
AC_SUBST(GETTEXT_PACKAGE, lightdm)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", Gettext package)

dnl ###########################################################################
dnl Files to generate
dnl ###########################################################################

AC_CONFIG_FILES([
Makefile
common/Makefile
data/Makefile
doc/Makefile
help/Makefile
liblightdm-gobject/liblightdm-gobject-1.pc
liblightdm-gobject/Makefile
liblightdm-qt/Makefile
liblightdm-qt/liblightdm-qt5-3.pc
po/Makefile.in
src/Makefile
tests/Makefile
tests/src/Makefile
tests/scripts/login-pam-config.conf
tests/scripts/login-pam.conf
tests/scripts/switch-to-greeter-return-session-pam.conf
])
AC_OUTPUT

dnl ###########################################################################
dnl Summary
dnl ###########################################################################

echo "
                    Light Display Manager $VERSION
                    ===========================

        prefix:                   $prefix
        Greeter session:          $DEFAULT_GREETER_SESSION
        Greeter user:             $GREETER_USER
        User session:             $DEFAULT_USER_SESSION
        GObject introspection:    $found_introspection
        Vala bindings:            $enable_vala
        liblightdm-qt5:           $compile_liblightdm_qt5
        libaudit support:         $use_libaudit
        Enable tests:             $enable_tests
"
