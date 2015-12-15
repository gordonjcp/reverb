/*

ladspa.c
ladspa interface for a simple reverb plugin

Copyright (c) 2015 Gordon JC Pearce <gordonjcp@gjcp.net>

Permission to use, copy, modify, and/or distribute this software for any purpose
with or without fee is hereby granted, provided that the above copyright notice
and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.

*/
#include <stdlib.h>

#include "ladspa.h"
#include "reverb.h"


/* temporary */
#define INPUT 0
#define OUTPUT 1

static LADSPA_Descriptor *reverb_descriptor = NULL;

void _init(void);
void _fini(void);

typedef struct {
    LADSPA_Data *input;
    LADSPA_Data *output;
} Reverb;

const LADSPA_Descriptor *ladspa_descriptor(unsigned long index) {
    switch (index) {
        case 0: return reverb_descriptor;
        default: return NULL;
    }
}

static void cleanup(LADSPA_Handle instance) {
    free(instance);
}

static void connect_port(LADSPA_Handle instance, unsigned long port, LADSPA_Data *data) {
    Reverb *plugin = (Reverb *) instance;

    switch(port) {
        case INPUT:
            plugin->input = data;
            break;
        case OUTPUT:
            plugin->output = data;
            break;
    }
}

static LADSPA_Handle instantiate(
    const LADSPA_Descriptor *descriptor __attribute__((unused)),
    unsigned long rate __attribute__((unused))) {
    Reverb *plugin = (Reverb *)malloc(sizeof(Reverb));



    /* in here we should set up any initial state */
    return (LADSPA_Handle)plugin;
}

static void run_reverb(LADSPA_Handle instance, unsigned long sample_count) {

    Reverb *plugin = (Reverb *)instance;
    /* connections */
    LADSPA_Data * const input = plugin->input;
    LADSPA_Data * const output = plugin->output;

    reverb(input, output, sample_count);

}

void _init(void) {
    /* called when plugin is loaded */
    char **port_names;
    LADSPA_PortDescriptor *port_descriptors;
    LADSPA_PortRangeHint *port_range_hints;

    reverb_descriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));

    if (reverb_descriptor) {
        reverb_descriptor->UniqueID = 250; /* "experimental" ID range */
        reverb_descriptor->Label = "reverb";
        reverb_descriptor->Properties = LADSPA_PROPERTY_HARD_RT_CAPABLE;
        reverb_descriptor->Name = "Simple Reverb";
        reverb_descriptor->Maker = "Gordonjcp <gordonjcp@gjcp.net";
        reverb_descriptor->Copyright = "ISC";
        reverb_descriptor->PortCount = 2;   /* obviously we'll need more */

        /* allocate port descriptors */
        port_descriptors = (LADSPA_PortDescriptor *)calloc(2, sizeof(LADSPA_PortDescriptor));
        reverb_descriptor->PortDescriptors = (const LADSPA_PortDescriptor *)port_descriptors;

        port_range_hints = (LADSPA_PortRangeHint *)calloc(2, sizeof(LADSPA_PortRangeHint));
        reverb_descriptor->PortRangeHints = (const LADSPA_PortRangeHint *)port_range_hints;
        port_names = (char **)calloc(2, sizeof(char*));
        reverb_descriptor->PortNames = (const char **)port_names;

        /* define the ports */
        /* temporary - only have input and output for now */
        port_descriptors[INPUT] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
        port_names[INPUT] = ("Input");
        port_range_hints[INPUT].HintDescriptor = 0;

        port_descriptors[OUTPUT] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
        port_names[OUTPUT] = ("Output");
        port_range_hints[OUTPUT].HintDescriptor = 0;

        /* set the various callbacks */
        reverb_descriptor->activate = NULL;
        reverb_descriptor->cleanup = cleanup;
        reverb_descriptor->connect_port = connect_port;
        reverb_descriptor->deactivate = NULL;
        reverb_descriptor->instantiate = instantiate;
        reverb_descriptor->run = run_reverb;
        reverb_descriptor->run_adding = NULL;
        reverb_descriptor->set_run_adding_gain = NULL;

    }
}

void _fini(void) {
        if (reverb_descriptor) {
                free((LADSPA_PortDescriptor *)reverb_descriptor->PortDescriptors);
                free((char **)reverb_descriptor->PortNames);
                free((LADSPA_PortRangeHint *)reverb_descriptor->PortRangeHints);
                free(reverb_descriptor);
        }
}
