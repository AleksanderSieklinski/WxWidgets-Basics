#include "Frame.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/filefn.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/dcmemory.h>
#include <wx/dcscreen.h>
#include <wx/dcclient.h>
#include <wx/dcgraph.h>
#include <wx/graphics.h>
#include <wx/wfstream.h>
int temp = 0;
class Picture : public Frame1
{
    wxColor kolor=wxColor(0,0,0);
    wxBitmap bitmap;
    wxBitmap bananbitmap;
    void rys() {
            std::unique_ptr<wxClientDC> cdc(new wxClientDC(this));
            bitmap = wxBitmap(this->GetSize());
            std::unique_ptr<wxBufferedDC> bdc(new wxBufferedDC(cdc.get(), bitmap));

            bdc->SetDeviceOrigin(this->GetSize().x/2, this->GetSize().y/2);
            wxColour tlo=*wxLIGHT_GREY;
            bdc->SetBackground(tlo);
            bdc->Clear();

            double sinbanan = 0.05 * gauge_banan->GetValue() * sin(gauge_banan->GetValue()*M_PI / 100);

            if (checkbox_banan->IsChecked())
                bdc->DrawBitmap(bananbitmap, -40 - sinbanan - bananbitmap.GetWidth() / 2, 30 - 0.4 * gauge_banan->GetValue() - bananbitmap.GetWidth() / 2, true);

            bdc->DrawLine(0, 0, 0, 60);
            bdc->DrawLine(0, 10, -40 - (0.1 * gauge_banan->GetValue() * sin(gauge_banan->GetValue()*M_PI / 100)), 30 - 0.4 * gauge_banan->GetValue());
            bdc->DrawLine(0, 10, 40, 30);
            bdc->DrawLine(0, 60, -30, 100);
            bdc->DrawLine(0, 60, 30, 100);

            bdc->DrawCircle(0, -20, 20);

            bdc->DrawEllipse(-12, -29, 9, 6);
            bdc->DrawEllipse(4, -29, 9, 6);
            if (checkbox_banan->IsChecked())
            {
                bdc->DrawEllipticArc(-16, -16, 28, 5, 180, 360);
            } else
            {
                bdc->DrawEllipticArc(-16, -16, 28, 5, 0, 180);
            }
            bdc->SetPen(tlo);
            if(comboBox_star->GetSelection()==2) {
                wxPoint gwiazda[5] = {wxPoint(-65, -85),wxPoint(-40, -40),wxPoint(-95, -65),wxPoint(-35, -65),wxPoint(-85, -40)};
                bdc->SetBrush(wxBrush(kolor));
                bdc->DrawPolygon(5, gwiazda);
            }
            else if(comboBox_star->GetSelection()==0) {
                bdc->SetBrush(wxBrush(*wxGREY_BRUSH));
                bdc->DrawCircle(-70, -60, -20);
                bdc->SetBrush(wxBrush(tlo));
                bdc->DrawCircle(-80, -70, -20);
            }
            else if(comboBox_star->GetSelection()==1) {
                bdc->SetBrush(wxBrush(*wxYELLOW));
                bdc->DrawCircle(-70, -60, -20);
            }
            wxString text(tekst->GetValue());
            bdc->DrawText(text, -90, 100);
            wxString texthoriz(tekst->GetValue());
            bdc->SetFont(wxFont(40, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial"));
            bdc->DrawRotatedText(tekst->GetValue(), 50, 100, 90);
        }
    protected:
        virtual void Frame1_OnPaint( wxPaintEvent& event ) override{rys();}   
        virtual void Frame1_OnUpdateUI( wxUpdateUIEvent& event ) override{
            rys(); 
            if (checkbox_banan->IsChecked()) {
                if (slider_banan->GetValue() == 100) {
                temp=0;
            }
            else if (slider_banan->GetValue() == 0) {
                temp=1;
            }
            if(temp==1) {
                slider_banan->SetValue(slider_banan->GetValue() + 1);
                gauge_banan->SetValue(slider_banan->GetValue()+1);
            }
            else if(temp==0) {
                slider_banan->SetValue(slider_banan->GetValue() - 1);
                gauge_banan->SetValue(slider_banan->GetValue()-1);
            }
            }
        }
        virtual void OnClickSave(wxCommandEvent& event) override{
            wxFileDialog saveFileDialog(panel1, _("Zapisz plik"), "", "Pliklab03","Pliki JPEG (*.jpeg)|*.jpeg", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
            if (saveFileDialog.ShowModal() == wxID_CANCEL)
                return;
            wxFileOutputStream output_stream(saveFileDialog.GetPath());
            if (!output_stream.IsOk())
            {
                wxLogError("Nie można zapisać pliku");
                return;
            }
            bitmap.SaveFile(saveFileDialog.GetPath(), wxBITMAP_TYPE_JPEG);
        }
        virtual void OnClickCheckBanana(wxCommandEvent& event) override{
            if (slider_banan->IsEnabled()) {
                slider_banan->Disable();
                slider_banan->SetValue(0);
            }
            else {
                slider_banan->Enable();
                slider_banan->SetValue(0);
                gauge_banan->SetValue(0);
            }
            rys();
        }
        virtual void Onscroll_banan(wxScrollEvent& event) override{
            gauge_banan->SetValue(slider_banan->GetValue());
            rys();
        }
        virtual void OnClickColor(wxCommandEvent& event) override{
            wxColourDialog dialog(this);
            if (dialog.ShowModal() == wxID_OK) {
                kolor = dialog.GetColourData().GetColour();
                tekst->SetForegroundColour(kolor);
            }
            rys();
        }
        virtual void comboBox_star_OnCombobox( wxCommandEvent& event ) override{event.Skip();}
        virtual void OnFilePicker( wxFileDirPickerEvent& event ) override{event.Skip();}
    public:
        Picture() : Frame1(NULL) {
            wxImage image;
            wxInitAllImageHandlers();
            image.LoadFile("banan.png");
            image.Rescale(50,50);
            bananbitmap = wxBitmap(image);
            if (bananbitmap.IsOk()) {
                std::cout << "bananbitmap is ok" << std::endl;
            }
        }
        ~Picture() {}

};