// GUBS.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GUBS.h"

#include "UnitAspects\ActionDetractor.h"
#include "UnitAspects\UnitSupplyAspect.h"


#include "Supply\SupplyTypes\SupplyTypeDefinition.h"
#include "Supply\SupplyConsumer.h"
#include "Supply\SupplyConsumption.h"
#include "Supply\SupplyContainer.h"
#include "Supply\SupplyDefinitionCatalog.h"
#include "Supply\SupplyRequirement.h"
#include "Supply\UnitSupplyElement.h"
#include "Supply\SupplyScopeAnswer.h"
#include "SupportClasses\Volume.h"



using namespace GUBS_Supply;
using namespace GUBS_Support;
using namespace GUBS_Enums;
using namespace GUBS_UnitAspects;

using std::unique_ptr;
#define MAX_LOADSTRING 100


UnitSupplyElement CreateUnitFuelSupplyElement(const SupplyTypeDefinition& supplyDef);
UnitSupplyElement CreateUnitOilSupplyElement(const SupplyTypeDefinition& supplyDef);
void SetDieselFuelRequirements(UnitSupplyElement& unitSupply);
void SetLubOilRequirements(UnitSupplyElement& unitSupply);


_CrtMemState sOld;
_CrtMemState sNew;
_CrtMemState sDiff;

void MemorySnapshot()
{
    OutputDebugString(L"-----------_CrtMemCheckpoint ---------\n");
    _CrtMemCheckpoint(&sOld); //take a snapchot
}
void MemoryUseReport()
{
    _CrtMemCheckpoint(&sNew); //take a snapchot 
    if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
    {
        OutputDebugString(L"-----------_CrtMemDumpStatistics ---------\n");
        _CrtMemDumpStatistics(&sDiff);
        OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------\n");
        _CrtMemDumpAllObjectsSince(&sOld);
        OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------\n");
        _CrtDumpMemoryLeaks();
    }
}


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GUBS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUBS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUBS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GUBS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


void runTest()
{
    MemorySnapshot();
    // TODO: Place code here.
    std::unique_ptr<SupplyDefinitionCatalog> _SupplyCatalog = std::make_unique<SupplyDefinitionCatalog>();

    {

        //	1 litre of diesel fuel
        SupplyTypeDefinition supplyDef = _SupplyCatalog->CreateSupply("DIESEL", "Liquid Diesel Fuel", SupplyType::POWER,
            SupplySubType::LIQUID, MeasurementUnit::LITER, 0.83, Volume(.1, .1, .1));

        ////	1 litre of oil
        SupplyTypeDefinition supplyOilDef = _SupplyCatalog->CreateSupply("LUBRICATING OIL - 10w40", "Engine oil, multi-weight", SupplyType::LUBRICATION,
            SupplySubType::LIQUID, MeasurementUnit::LITER, 0.93, Volume(.1, .1, .1));

        UnitSupplyElement supply = CreateUnitFuelSupplyElement(supplyDef);
        supply.AddSupplyContainers(1.0);
        UnitSupplyElement oilSupply = CreateUnitOilSupplyElement(supplyOilDef);
        oilSupply.AddSupplyContainers(1.0);

        UnitSupplyAspect unitSupplyAspect;
        unitSupplyAspect.AddSupplyElement(supply);
        unitSupplyAspect.AddSupplyElement(oilSupply);

        //	set up supply consumption drivers
        SupplyConsumptionQuestion drivers;
        //	Travel 10 km
        drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::METER, 10000));
        //	Travel at 10 m/s => 36 km/hr
        drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::METER_PER_SECOND, 10));
        //	Travel for ~15 min
        drivers.AddConsumptionDriver(UnitizedValue(MeasurementUnit::SECOND, 1000));

        //
        int count = 0;
        while (!unitSupplyAspect.IsDepleted(supplyDef))
        {
            SupplyConsumptionQuestionAnswer answer = unitSupplyAspect.Consume(drivers);
            count++;
        }

        std::vector<UnitSupplyElement> testSupply = unitSupplyAspect.GetSupplyOfType(supplyDef.Type());
        UnitSupplyElement element = testSupply[0];

        testSupply = unitSupplyAspect.GetSupplyOfType(SupplyType::LUBRICATION);

        OutputDebugString(L"-----------AddUnitSupply ---------");
    }
    MemoryUseReport();
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                runTest();
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	the unit fuel supply element is defined as 1000 liters in a rigid container
//	it is consumed at 1 liter/10 km base with a velocity dependent
//	consumption of 1/10000 liter times the square of the velocity
//	and 1/100000 of a liter per second of run time (to account for idle consumption)
UnitSupplyElement CreateUnitFuelSupplyElement(const SupplyTypeDefinition& supplyDef)
{
    //	take the supplyDef and consume it in units of LITERS
    UnitSupplyElement unitSupply(supplyDef, MeasurementUnit::LITER);

    //	1 litre supply quantity
    //	Requirement container of 1000 x 1 litre supply quantity (1000 litre)
    unitSupply.IntializeRequirementContainer(1.0, SupplyContainerType::RIGID, 1000.0);
    //	Consume 1/10000 of a litre per meter
    unitSupply.AddConsumption(MeasurementUnit::METER, 0.0001, 1.0);
    //	Consume an additional 1/10000 of a litre per (meter per second) ^ 2
    unitSupply.AddConsumption(MeasurementUnit::METER_PER_SECOND, 0.0001, 2.0);
    //	Consume 1/100000 of a litre per second
    unitSupply.AddConsumption(MeasurementUnit::SECOND, 0.000001, 1.0);
    SetDieselFuelRequirements(unitSupply);

    return unitSupply;
}


//	unit lubricating oil supply element is defined a 10 liters in a rigid container
//	it is consumed at 1/100000000 of a litre per meter
//	and 1/1000000000 of a litre per second of run time
UnitSupplyElement CreateUnitOilSupplyElement(const SupplyTypeDefinition& supplyDef)
{
    UnitSupplyElement unitSupply(supplyDef, MeasurementUnit::LITER);

    //	1 litre supply quantity
    //	Requirement container of 10 x 1 litre supply quantity (10 litre)
    unitSupply.IntializeRequirementContainer(1.0, SupplyContainerType::RIGID, 10.0);
    //	Consume 1/100000000 of a litre per meter
    unitSupply.AddConsumption(MeasurementUnit::METER, 0.00000001, 1.0);
    //	Consume 1/1000000000 of a litre per second
    unitSupply.AddConsumption(MeasurementUnit::SECOND, 0.000000001, 1.0);
    SetLubOilRequirements(unitSupply);

    return unitSupply;
}

void SetDieselFuelRequirements(UnitSupplyElement& unitSupply)
{
    //	Full, restricted and minimal supply levels are indicated but have no detractors
    //	consider fully supplied up to 1000 litre (Fully supplied over 100 litre)
    unitSupply.SetSupplyLevel(SupplyLevel::SUPPLIED, 100, 0.0, 0.0, 0.0);
    //	consider restricted supply up to 50 litre (restricted supply 50 - 100 litre)
    unitSupply.SetSupplyLevel(SupplyLevel::RESTRICTED, 50, 0.0, 0.0, 0.0);
    //	consider minimally supplied up to 0 litre (minimal supply 0 - 50 litre)
    unitSupply.SetSupplyLevel(SupplyLevel::MINIMAL, 1.0, 0.0, 0.0, 0.0);
    //	consider unsupplied below 0 litre
    //	Unsupplied creates a 100% movement detractor (no fuel)
    unitSupply.SetSupplyLevel(SupplyLevel::UNSUPPLIED, 0, 1.0, 0.0, 0.0);

    //	The result of running out of fuel, requiring resupply, occurs 0 seconds after supply is exhausted
    unitSupply.SetUnsuppliedOutcome(UnsuppliedOutcome::REQUIRES_RESUPPLY, 0, MeasurementUnit::SECOND);
}

void SetLubOilRequirements(UnitSupplyElement& unitSupply)
{
    //	Full, restricted and minimal supply levels are indicated but have no detractors
    //	consider fully supplied up to 10 litre (Fully supplied over 5 litre)
    unitSupply.SetSupplyLevel(SupplyLevel::SUPPLIED, 5, 0.0, 0.0, 0.0);
    //	consider restricted supply up to 2 litre (restricted supply 1 - 2 litre)
    unitSupply.SetSupplyLevel(SupplyLevel::RESTRICTED, 2, 0.0, 0.0, 0.0);
    //	consider minimally supplied up to 1 litre (minimal supply 0.5 - 1 litre)
    unitSupply.SetSupplyLevel(SupplyLevel::MINIMAL, 1, 0.50, 0.0, 0.0);
    //	consider unsupplied up to 0 litre
    //	Unsupplied creates a 100% movement detractor (no oil)
    unitSupply.SetSupplyLevel(SupplyLevel::UNSUPPLIED, 0.5, 1.0, 0.0, 0.0);

    //	The result of running out of oil, requiring resupply and repair, occurs 60 seconds after supply is exhausted
    UnsuppliedOutcome result = UnsuppliedOutcome::REQUIRES_RESUPPLY | UnsuppliedOutcome::REQUIRES_REPAIR;
    bool res = hasFlag(result, UnsuppliedOutcome::REQUIRES_REPAIR);
    unitSupply.SetUnsuppliedOutcome(result, 60, MeasurementUnit::SECOND);
}