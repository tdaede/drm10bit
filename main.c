#include <stdio.h>
#include <stdlib.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

int main(int argc, char * argv[])
{
  int control_minor = 64;
  int c = drmOpenControl(control_minor);
  if (!c) {
    printf("DRM open device %d failed\n", control_minor);
    abort();
  }
  int render_minor = 128;
  int r = drmOpenRender(render_minor);
  if (!r) {
    printf("DRM open render node %d failed\n", render_minor);
  }
  drmVersionPtr v = drmGetVersion(r);
  if (!v) {
    printf("Couldn't get DRM version\n");
    abort();
  }
  printf("%s %s %s\n", v->name, v->date, v->desc);
  drmModeResPtr moderes = drmModeGetResources(c);
  if (!moderes) {
    printf("Couldn't get DRM modesetting resources\n");
    abort();
  }
  printf("fbs: %d crtcs: %d connectors: %d encoders: %d\n",
   moderes->count_fbs, moderes->count_crtcs, moderes->count_connectors, moderes->count_encoders);
  for (int i = 0; i < moderes->count_connectors; i++) {
    printf("Connector %d\n", moderes->connectors[i]);
    drmModeConnectorPtr connector = drmModeGetConnector(c, moderes->connectors[i]);
    if (connector->connection == DRM_MODE_CONNECTED) {
      printf("\tConnected\n");
      printf("\tSize: %d x %d mm\n", connector->mmWidth, connector->mmHeight);
      printf("\tModes:\n");
      for (int j = 0; j < connector->count_modes; j++) {
        printf("\t\t%s\n", connector->modes[j].name);
      }
      printf("\tProperties:\n");
      for (int j = 0; j < connector->count_props; j++) {
        drmModePropertyPtr property = drmModeGetProperty(c, connector->props[j]);
        printf("\t\t%s\n", property->name);
      }
    } else {
      printf("\tDisconnected\n");
    }
  }
}
