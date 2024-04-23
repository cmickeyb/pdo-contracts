# Copyright 2024 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import base64
import logging
import os

import ipywidgets

_logger = logging.getLogger(__name__)

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class FileDownloadLink(ipywidgets.HTML) :
    """Create a HTML reference to download a file that is not in the
    Jupyter server namespace.

    The content of the file is read, base64 encoded and embedded in the
    address reference. This class can serve as a base class for
    alternative ways of displaying the link.
    """

    _address_start_template_ = '<a download="{filename}" href="data:{file_type};base64,{payload}" download>'
    _label_template_ = '{label}'
    _address_end_template_ = '</a>'

    _extensions_ = {
        '.pem' : 'application/x-pem-file',
        '.pdo' : 'application/json',
        '.toml' : 'application/toml',
        '.zip' : 'application/zip',
    }

    def __init__(self, filename : str, label : str = "Download File") :
        with open(filename, "rb") as fp :
            payload = base64.b64encode(fp.read()).decode()

        file_type = self._extensions_.get(os.path.splitext(filename)[1], 'application/octet-stream')
        template = self._address_start_template_ + self._label_template_ + self._address_end_template_

        parameters = {
            'file_type' : file_type,
            'label' : label,
            'payload' : payload,
            'filename' : os.path.basename(filename),
        }

        content = template.format(**parameters)
        super().__init__(value=content)

class FileDownloadButton(FileDownloadLink) :
    """Create a Jupyter button for a download link"""
    _label_template_ = '<button class="p-Widget jupyter-widgets jupyter-button widget-button mod-warning">{label}</button>'

# -----------------------------------------------------------------
# -----------------------------------------------------------------
class ImportContractWidget(ipywidgets.VBox) :
    """Define a widget class for uploading a file

    This widget consists of several components: the file, an upload
    button, and a feedback area.
    """
    def __init__(self, state : pbuilder.state.State, bindings : pbuilder.bindings.Bindings) :
        self.state = state
        self.bindings = bindings

        self.file = ipywidgets.FileUpload(accept='*', description='Select File')

        self.upload_button = ipywidgets.Button(description="Upload")
        self.upload_button.on_click(self.upload_button_click)

        self.feedback = ipywidgets.Output()

        hbox = ipywidgets.HBox([self.file, self.upload_button])
        super().__init__([hbox, self.feedback])

    def reset_widget(self) :
        """Reset the values of the components in the widget
        """
        self.file.value = ()

    def upload_button_click(self, b) :
        """Handle the request to upload the file
        """
        self.feedback.clear_output()

        if not self.entry.value or not self.entry.value.isalnum() :
            with self.feedback : print("Identity must be alphanumeric")
            return
        if not self.file.value :
            with self.feedback : print("No file specified")
            return

        keydir = self.bindings.expand("${keys}")
        keyfile = "{}_{}.pem".format(self.entry.value, self.type.value)
        filename = os.path.join(keydir, keyfile)
        with open(filename, "wb") as fp : fp.write(self.file.value[0].content)
        with self.feedback : print("keys uploaded to {}".format(filename))

        self.reset_widget()
