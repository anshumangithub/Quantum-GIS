INSTALL(FILES srs.db qgis.db qgis_help.db symbology-ng-style.db spatialite.db customization.xml
        DESTINATION ${QGIS_DATA_DIR}/resources)

ADD_SUBDIRECTORY(context_help)
ADD_SUBDIRECTORY(function_help)
