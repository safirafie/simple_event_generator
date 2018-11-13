#include <TObject.h>

class MyEvent : public TObject
  {
  public:
    Int_t nTracks;
    Double_t fV2;
    MyEvent();

    ClassDef(MyEvent, 1); // Help class
  };

ClassImp(MyEvent) // No semi-colon!

MyEvent::MyEvent():
TObject(),
  nTracks(-1),
  fV2(-1)
{
}

class MyTrack : public TObject
{
public:
  Double_t fPhi;
  MyTrack();

  ClassDef(MyTrack, 1); // Help class
};

ClassImp(MyTrack) // No semi-colon!

MyTrack::MyTrack():
TObject(),
  fPhi(-1)
{
}
