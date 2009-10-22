// {{{ Copyright notice
/* Copyright (c) 2007, Adam Harvey
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *  - The names of its contributors may not be used to endorse or promote
 *  products derived from this software without specific prior written
 *  permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
// }}}

#include "ArtProvider.h"
#include "images.h"

#include <wx/image.h>
#include <wx/log.h>
#include <wx/mstream.h>

// {{{ wxBitmap ArtProvider::CreateBitmap(const wxArtID &id, const wxArtClient &client, const wxSize &size)
wxBitmap ArtProvider::CreateBitmap(const wxArtID &id, const wxArtClient &client, const wxSize &size) {
	const Images::Image *img = Images::GetImage(id);
	if (img == NULL) {
		return wxNullBitmap;
	}

	wxMemoryInputStream mis(img->image, img->size);
	wxImage image(mis, wxBITMAP_TYPE_PNG);

	/* There seems to be a tendency for wxArtProvider to request images of
	 * size (-1, -1), so we need to avoid trying to rescale for them. */
	if (wxSize(image.GetWidth(), image.GetHeight()) != size && size.GetWidth() > 0 && size.GetHeight() > 0) {
		wxLogDebug(wxT("Requested width: %d; height: %d"), size.GetWidth(), size.GetHeight());
		image.Rescale(size.GetWidth(), size.GetHeight(), wxIMAGE_QUALITY_HIGH);
	}

	return wxBitmap(image);
}
// }}}

// vim:set fdm=marker ts=8 noet cin:
