package=wolfssl
$(package)_version=4.5.0
$(package)_download_path=https://github.com/wolfSSL/wolfssl/archive
$(package)_download_file=v$($(package)_version)-stable.tar.gz
$(package)_file_name=wolfssl-$($(package)_version).tar.gz
$(package)_sha256_hash=7de62300ce14daa0051bfefc7c4d6302f96cabc768b6ae49eda77523b118250c

define $(package)_set_vars
$(package)_config_env=AR="$($(package)_ar)" RANLIB="$($(package)_ranlib)" CC="$($(package)_cc)"
$(package)_config_opts=--prefix=$(host_prefix)
$(package)_config_opts+=--host=$(host)
$(package)_config_opts+=--enable-static
$(package)_config_opts+=--disable-shared
$(package)_config_opts+=--disable-examples
$(package)_config_opts+=--disable-crypttests
$(package)_config_opts+=--enable-keygen
$(package)_config_opts+=--enable-certgen
$(package)_config_opts+=--enable-enckeys
$(package)_config_opts+=--enable-opensslall
$(package)_config_opts+=--enable-opensslextra
$(package)_config_opts+=--enable-harden
endef

define $(package)_preprocess_cmds
  cd $($(package)_build_subdir); ./autogen.sh
endef

define $(package)_config_cmds
  ./configure $($(package)_config_opts)
endef

#define $(package)_config_cmds
#  $($(package)_autoconf)
#endef

define $(package)_build_cmds
  $(MAKE) -j1 src/libwolfssl.la
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install-libLTLIBRARIES install-includeHEADERS install-nobase_includeHEADERS install-pkgconfigDATA
endef

#define $(package)_postprocess_cmds
#  rm -rf bin share
#endef

