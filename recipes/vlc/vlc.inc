DESCRIPTION = "Video player and streamer - davinci edition"
HOMEPAGE = "http://www.videolan.org"
LICENSE = "GPL"
PRIORITY = "optional"
SECTION = "multimedia"

DEPENDS = "libfribidi libtool hal gettext libgcrypt schroedinger virtual/libsdl qt4-x11-free dbus libxml2 gnutls tremor faad2 ffmpeg flac \
           ${@base_conditional('ENTERPRISE_DISTRO', '1', '', 'libmad libid3tag liba52 mpeg2dec', d)}"

INC_PR = "r4"

SRC_URI = "http://download.videolan.org/pub/videolan/vlc/${PV}/vlc-${PV}.tar.bz2"

inherit autotools

ARM_INSTRUCTION_SET = "arm"

EXTRA_OECONF = "\
	--enable-libtool \
	--with-contrib \
	--enable-run-as-root \
	--disable-wxwidgets \
	--enable-x11 --enable-xvideo \ 
	--disable-screen --disable-caca \
	--enable-httpd --enable-vlm \
	--enable-freetype \
	--enable-sdl \ 
	--enable-png \
	--enable-live555 --enable-tremor \
	--enable-v4l2 --enable-v4l --disable-aa --enable-wma --disable-faad \
	--enable-dbus \
	--enable-hal \	
	--without-contrib \
	--disable-opengl --disable-glx \
	--enable-realrtsp \
	ac_cv_path_MOC=${STAGING_BINDIR_NATIVE}/moc4 \
	ac_cv_path_RCC=${STAGING_BINDIR_NATIVE}/rcc4 \
	ac_cv_path_UIC=${STAGING_BINDIR_NATIVE}/uic4 \
"


do_configure() {
	cp ${STAGING_DATADIR}/aclocal/libgcrypt.m4 ${S}/m4/ 
	./bootstrap	
	gnu-configize --force
	libtoolize --force
	#autoreconf --force -i
	cp ${STAGING_DATADIR}/libtool/config.* ${S}/autotools/ || true
	oe_runconf
	rm config.log
	sed -i -e s:-L/usr/lib:-L${STAGING_LIBDIR}/:g vlc-config
	sed -i -e s:'$(MOC) $(DEFS) $(CPPFLAGS)':'$(MOC) $(DEFS)'\ -I${S}/include\ -DSYS_LINUX:g ${S}/modules/gui/qt4/Makefile
}

FILES_${PN} += "${bindir}/vlc \
	${datadir}/applications \
	${datadir}/vlc/ \
	"

FILES_${PN} += "${datadir}/icons"
FILES_${PN}-dbg	+= "${libdir}/vlc/*/.debug ${libdir}/vlc/plugins/*/.debug"
