include $(TOPDIR)/rules.mk

PKG_NAME:=letmecreate
PKG_VERSION:=0.1

PKG_LICENSE:=BSD-3-Clause
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/letmecreate
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=LetMeCreate
endef

CMAKE_OPTIONS = \
	-DBUILD_EXAMPLES=ON

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) LetMeCreate/* $(PKG_BUILD_DIR)/
	rm -Rf $(PKG_BUILD_DIR)/CMakeFiles/
	rm -f $(PKG_BUILD_DIR)/CMakeCache.txt
	rm -f $(PKG_BUILD_DIR)/Makefile
	rm -f $(PKG_BUILD_DIR)/cmake_install.cmake
	rm -Rf $(PKG_BUILD_DIR)/lib/
	rm -Rf $(PKG_BUILD_DIR)/examples/CMakeFiles/
	rm -f $(PKG_BUILD_DIR)/examples/Makefile
	rm -f $(PKG_BUILD_DIR)/examples/cmake_install.cmake
	rm -Rf $(PKG_BUILD_DIR)/examples/bin
endef

define Package/letmecreate/install
	mkdir -p $(1)/usr/bin/letmecreate_examples
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/bin/* $(1)/usr/bin/letmecreate_examples

	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/lib/*.so $(1)/usr/lib/
endef

$(eval $(call BuildPackage,letmecreate))
