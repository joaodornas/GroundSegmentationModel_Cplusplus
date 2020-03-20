#pragma once

#pragma warning (disable: 4244)
#pragma warning (disable: 4996)
#pragma warning (disable: 4018)
#pragma warning (disable: 4267)

#include "Globals.h"
#include "Result.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////
// LOG INFORMATION ///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void RegisterStep(std::string event, bool verbose, std::string msg, int iStep)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// REGISTER EACH LOOP STEP ///////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	SaveSanityCheckDims(_LOGINFOOPTIONS.doSanityCheckDim, msg, event);

	auto current_time = std::chrono::system_clock::now();
	auto duration_in_seconds = std::chrono::duration<double>(current_time.time_since_epoch());
	double num_seconds = duration_in_seconds.count();
	
	if (msg == _LOGINFOOPTIONS.codeSteps.RunModelLoop)
	{
		if (event == "start")
		{
			_LOGINFOOPTIONS.cputime_RunModelLoop.start = num_seconds;
			_LOGINFOOPTIONS.cputime_RunModelLoop.count++;
		}
		else if (event == "stop")
		{
			_LOGINFOOPTIONS.cputime_RunModelLoop.finish = num_seconds;
			_LOGINFOOPTIONS.cputime_RunModelLoop.elapsed += _LOGINFOOPTIONS.cputime_RunModelLoop.finish - _LOGINFOOPTIONS.cputime_RunModelLoop.start;

		}
	}

	if (msg == _LOGINFOOPTIONS.codeSteps.SectorsLoop)
	{
		if (event == "start")
		{
			_LOGINFOOPTIONS.cputime_SectorsLoop.start = num_seconds;
			_LOGINFOOPTIONS.cputime_SectorsLoop.count++;
		}
		else if (event == "stop")
		{
			_LOGINFOOPTIONS.cputime_SectorsLoop.finish = num_seconds;
			_LOGINFOOPTIONS.cputime_SectorsLoop.elapsed += _LOGINFOOPTIONS.cputime_SectorsLoop.finish - _LOGINFOOPTIONS.cputime_SectorsLoop.start;

		}
	}

	if (msg == _LOGINFOOPTIONS.codeSteps.GaussLoop)
	{
		if (event == "start")
		{
			_LOGINFOOPTIONS.cputime_GaussLoop.start = num_seconds;
			_LOGINFOOPTIONS.cputime_GaussLoop.count++;
		}
		else if (event == "stop")
		{
			_LOGINFOOPTIONS.cputime_GaussLoop.finish = num_seconds;
			_LOGINFOOPTIONS.cputime_GaussLoop.elapsed += _LOGINFOOPTIONS.cputime_GaussLoop.finish - _LOGINFOOPTIONS.cputime_GaussLoop.start;

		}
	}

	std::string division_str = "--------------------------------------------------";

	if ( (_LOGINFOOPTIONS.verbose) && (event == "start") )
	{
		
			std::cout << "<" << division_str << ">\n";
			std::cout << "<" + msg + ">:<" << iStep << ">\n";
			std::cout << "<" << division_str << ">\n";
		
	}

	division_str.clear();

};

void RegisterEvent(std::string event, std::string codeStep)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// REGISTER EACH CODE STEP TIME EXECUTION ////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	SaveSanityCheckDims(_LOGINFOOPTIONS.doSanityCheckDim, codeStep, event);

	std::cout.precision(5);

	auto current_time = std::chrono::system_clock::now();
	auto duration_in_seconds = std::chrono::duration<double>(current_time.time_since_epoch());
	double num_seconds = duration_in_seconds.count();
		
	if (_LOGINFOOPTIONS.showReport || _LOGINFOOPTIONS.verbose || _LOGINFOOPTIONS.saveReport)
	{
			
		if (codeStep == _LOGINFOOPTIONS.codeSteps.PolarGridFunction)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_PolarGridFunction.start = num_seconds;
				_LOGINFOOPTIONS.cputime_PolarGridFunction.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_PolarGridFunction.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_PolarGridFunction.elapsed += _LOGINFOOPTIONS.cputime_PolarGridFunction.finish - _LOGINFOOPTIONS.cputime_PolarGridFunction.start;
					
				if (_LOGINFOOPTIONS.verbose)
				{ 
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_PolarGridFunction.elapsed << " ms>\n" << std::scientific;
				}

				
			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.sectorFunction)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_sectorFunction.start = num_seconds;
				_LOGINFOOPTIONS.cputime_sectorFunction.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_sectorFunction.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_sectorFunction.elapsed += _LOGINFOOPTIONS.cputime_sectorFunction.finish - _LOGINFOOPTIONS.cputime_sectorFunction.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_sectorFunction.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.getMinZFunction)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_getMinZFunction.start = num_seconds;
				_LOGINFOOPTIONS.cputime_getMinZFunction.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_getMinZFunction.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_getMinZFunction.elapsed += _LOGINFOOPTIONS.cputime_getMinZFunction.finish - _LOGINFOOPTIONS.cputime_getMinZFunction.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_getMinZFunction.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.countSeedFunction)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_countSeedFunction.start = num_seconds;
				_LOGINFOOPTIONS.cputime_countSeedFunction.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_countSeedFunction.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_countSeedFunction.elapsed += _LOGINFOOPTIONS.cputime_countSeedFunction.finish - _LOGINFOOPTIONS.cputime_countSeedFunction.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_countSeedFunction.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.extractSeedFunction)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_extractSeedFunction.start = num_seconds;
				_LOGINFOOPTIONS.cputime_extractSeedFunction.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_extractSeedFunction.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_extractSeedFunction.elapsed += _LOGINFOOPTIONS.cputime_extractSeedFunction.finish - _LOGINFOOPTIONS.cputime_extractSeedFunction.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_extractSeedFunction.elapsed << " ms>\n" << std::scientific;
				}


			}
		}


		if (codeStep == _LOGINFOOPTIONS.codeSteps.groupSpUSnewFunction)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_groupSpUSnewFunction.start = num_seconds;
				_LOGINFOOPTIONS.cputime_groupSpUSnewFunction.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_groupSpUSnewFunction.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_groupSpUSnewFunction.elapsed += _LOGINFOOPTIONS.cputime_groupSpUSnewFunction.finish - _LOGINFOOPTIONS.cputime_groupSpUSnewFunction.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_groupSpUSnewFunction.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.GetTestFunction)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_GetTestFunction.start = num_seconds;
				_LOGINFOOPTIONS.cputime_GetTestFunction.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_GetTestFunction.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_GetTestFunction.elapsed += _LOGINFOOPTIONS.cputime_GetTestFunction.finish - _LOGINFOOPTIONS.cputime_GetTestFunction.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_GetTestFunction.elapsed << " ms>\n" << std::scientific;
				}


			}
		}


		if (codeStep == _LOGINFOOPTIONS.codeSteps.Eval_PART1_STEPS1234)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.start = num_seconds;
				_LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.elapsed += _LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.finish - _LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.Eval_PART2_STEPS56)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.start = num_seconds;
				_LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.elapsed += _LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.finish - _LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.Eval_PART3_STEP7)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.start = num_seconds;
				_LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.elapsed += _LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.finish - _LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.Eval_PART4_STEP8)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.start = num_seconds;
				_LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.elapsed += _LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.finish - _LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.Eval_PART5_STEP9)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.start = num_seconds;
				_LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.elapsed += _LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.finish - _LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.Eval_PART6_STEP10)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.start = num_seconds;
				_LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.elapsed += _LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.finish - _LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.Eval_PART7A)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART7A.start = num_seconds;
				_LOGINFOOPTIONS.cputime_Eval_PART7A.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART7A.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_Eval_PART7A.elapsed += _LOGINFOOPTIONS.cputime_Eval_PART7A.finish - _LOGINFOOPTIONS.cputime_Eval_PART7A.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_Eval_PART7A.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.Eval_PART7B)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART7B.start = num_seconds;
				_LOGINFOOPTIONS.cputime_Eval_PART7B.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_Eval_PART7B.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_Eval_PART7B.elapsed += _LOGINFOOPTIONS.cputime_Eval_PART7B.finish - _LOGINFOOPTIONS.cputime_Eval_PART7B.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_Eval_PART7B.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.Segment_getHg)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_Segment_getHg.start = num_seconds;
				_LOGINFOOPTIONS.cputime_Segment_getHg.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_Segment_getHg.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_Segment_getHg.elapsed += _LOGINFOOPTIONS.cputime_Segment_getHg.finish - _LOGINFOOPTIONS.cputime_Segment_getHg.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_Segment_getHg.elapsed << " ms>\n" << std::scientific;
				}


			}
		}

		if (codeStep == _LOGINFOOPTIONS.codeSteps.Segment_doSegment)
		{
			if (event == "start")
			{
				_LOGINFOOPTIONS.cputime_Segment_doSegment.start = num_seconds;
				_LOGINFOOPTIONS.cputime_Segment_doSegment.count++;
			}
			else if (event == "stop")
			{
				_LOGINFOOPTIONS.cputime_Segment_doSegment.finish = num_seconds;

				_LOGINFOOPTIONS.cputime_Segment_doSegment.elapsed += _LOGINFOOPTIONS.cputime_Segment_doSegment.finish - _LOGINFOOPTIONS.cputime_Segment_doSegment.start;

				if (_LOGINFOOPTIONS.verbose)
				{
					std::cout << "<" + codeStep + ">:<" << _LOGINFOOPTIONS.cputime_Segment_doSegment.elapsed << " ms>\n" << std::scientific;
				}


			}
		}
			

	}

};

std::string showReportStep(std::string codeStep, double elapsed, int nRepetitions)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SHOW FINAL REPORT  ////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	double mTime = 0;
	
	if (nRepetitions == _DATA._zero_i)
	{
		mTime = _DATA._one_i;
	}
	else
	{
		mTime = (elapsed / nRepetitions);
	}

	std::string report_str = codeStep + "," + "<mean-time(s):>," + std::to_string(mTime) + "," + "<# of repetitions:>," + std::to_string(nRepetitions) + "," + "<total(s):>" + "," + std::to_string(elapsed) + "\n";

	return report_str;
	
};

void showReport()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SHOW FINAL REPORT /////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	std::string msg = "";
	std::string msg1;

	std::string codeStep;

	std::string division = "--------------------------------------------------------------------------------------------";
	std::string timeReport = "TIME REPORT";

	if ( (_LOGINFOOPTIONS.showReport) || (_LOGINFOOPTIONS.saveReport) )
	{

		codeStep = _LOGINFOOPTIONS.codeSteps.RunModelLoop;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_RunModelLoop.elapsed, _LOGINFOOPTIONS.cputime_RunModelLoop.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.SectorsLoop;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_SectorsLoop.elapsed, _LOGINFOOPTIONS.cputime_SectorsLoop.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.GaussLoop;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_GaussLoop.elapsed, _LOGINFOOPTIONS.cputime_GaussLoop.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.PolarGridFunction;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_PolarGridFunction.elapsed, _LOGINFOOPTIONS.cputime_PolarGridFunction.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.sectorFunction;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_sectorFunction.elapsed, _LOGINFOOPTIONS.cputime_sectorFunction.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.getMinZFunction;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_getMinZFunction.elapsed, _LOGINFOOPTIONS.cputime_getMinZFunction.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.extractSeedFunction;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_extractSeedFunction.elapsed, _LOGINFOOPTIONS.cputime_extractSeedFunction.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.groupSpUSnewFunction;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_groupSpUSnewFunction.elapsed, _LOGINFOOPTIONS.cputime_groupSpUSnewFunction.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.GetTestFunction;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_GetTestFunction.elapsed, _LOGINFOOPTIONS.cputime_GetTestFunction.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.Eval_PART1_STEPS1234;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.elapsed, _LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.Eval_PART2_STEPS56;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.elapsed, _LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.Eval_PART3_STEP7;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.elapsed, _LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.Eval_PART4_STEP8;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.elapsed, _LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.Eval_PART5_STEP9;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.elapsed, _LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.Eval_PART6_STEP10;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.elapsed, _LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.Eval_PART7A;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_Eval_PART7A.elapsed, _LOGINFOOPTIONS.cputime_Eval_PART7A.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.Eval_PART7B;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_Eval_PART7B.elapsed, _LOGINFOOPTIONS.cputime_Eval_PART7B.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.Segment_getHg;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_Segment_getHg.elapsed, _LOGINFOOPTIONS.cputime_Segment_getHg.count);
		msg = msg + msg1;

		codeStep = _LOGINFOOPTIONS.codeSteps.Segment_doSegment;
		msg1 = showReportStep(codeStep, _LOGINFOOPTIONS.cputime_Segment_doSegment.elapsed, _LOGINFOOPTIONS.cputime_Segment_doSegment.count);
		msg = msg + msg1;

		

	}

	if (_LOGINFOOPTIONS.showReport)
	{
		std::string file = "FILE:" + _DATAOPTIONS.fileName;

		std::cout << "<" + division + ">\n";
		std::cout << "<" + timeReport + ">\n";
		std::cout << "<" + file + ">\n";
		std::cout << "<" + division + ">\n";

		std::cout << msg;

		std::cout << "<" + division + ">\n";

		division.clear();
		timeReport.clear();

		
	}

	if (_LOGINFOOPTIONS.saveReport)
	{
		//create directory
		if (!std::experimental::filesystem::is_directory(_DATAOPTIONS.folderOutput + "\\Report\\") || !std::experimental::filesystem::exists(_DATAOPTIONS.folderOutput + "\\Report\\")) {
			std::experimental::filesystem::create_directory(_DATAOPTIONS.folderOutput + "\\Report\\");
		}

		std::ofstream out(_DATAOPTIONS.folderOutput + "\\Report\\" + _DATAOPTIONS.fileName + "-Report.txt", std::ofstream::out | std::ofstream::trunc);

		out << msg;

		out.close();
		

	}

	msg.clear();
	msg1.clear();
	

};

void clearTime()
{
	_LOGINFOOPTIONS.cputime_RunModelLoop.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_SectorsLoop.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_GaussLoop.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_SegmentLoop.count = _DATA._zero_i;

	_LOGINFOOPTIONS.cputime_PolarGridFunction.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_sectorFunction.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_getMinZFunction.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_countSeedFunction.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_extractSeedFunction.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_groupSpUSnewFunction.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_GetTestFunction.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_Eval_PART7A.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_Eval_PART7B.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_Segment_getHg.count = _DATA._zero_i;
	_LOGINFOOPTIONS.cputime_Segment_doSegment.count = _DATA._zero_i;

	_LOGINFOOPTIONS.cputime_RunModelLoop.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_SectorsLoop.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_GaussLoop.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_SegmentLoop.elapsed = _DATA._zero_f;

	_LOGINFOOPTIONS.cputime_PolarGridFunction.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_sectorFunction.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_getMinZFunction.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_countSeedFunction.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_extractSeedFunction.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_groupSpUSnewFunction.count = _DATA._one_i;
	_LOGINFOOPTIONS.cputime_GetTestFunction.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_Eval_PART7A.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_Eval_PART7B.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_Segment_getHg.elapsed = _DATA._zero_f;
	_LOGINFOOPTIONS.cputime_Segment_doSegment.elapsed = _DATA._zero_f;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// SANITY CHECK //////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void SaveSanityCheckSector(bool shouldDo, std::string codeStep, EVAL_struct & _eval, Sectors_struct & ocl_OP, size_t nModel, size_t nTest, size_t nOP, int inliers[])
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ( (_LOGINFOOPTIONS.doSanityCheck) && (shouldDo)  && ( (_LOGINFOOPTIONS.SanityCheckSector == _DATA._zero_i) || (_LOGINFOOPTIONS.SanityCheckSector == _DATA.iS) ) )
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string folder_SanityCheck = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";

		//create directory
		if (!std::experimental::filesystem::is_directory(folder_SanityCheck) || !std::experimental::filesystem::exists(folder_SanityCheck)) {
			std::experimental::filesystem::create_directory(folder_SanityCheck);
		}

		std::string folder_Sectors = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "Sectors\\";

		//create directory
		if (!std::experimental::filesystem::is_directory(folder_Sectors) || !std::experimental::filesystem::exists(folder_Sectors)) {
			std::experimental::filesystem::create_directory(folder_Sectors);
		}

		std::string outputFolder = folder_Sectors + "\\Sector-" + std::to_string(_DATA.iS) + "\\";
		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";
		std::string _fileName;

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		_eval.MapData();

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		//"K_X_star"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "K_X_star" + ".txt";

		saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_K_X_star_m, nModel, nTest);

		//"K_star_X"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "K_star_X" + ".txt";

		saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_K_star_X_m, nTest, nModel);

		//"K_star_star"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "K_star_star" + ".txt";

		saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_K_star_star_m, nTest, nTest);

		//"K_X_X"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "K_X_X" + ".txt";

		saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_K_X_X_m, nModel, nModel);

		//"Inverse"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "Inverse" + ".txt";

		saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_Inverse_m, nModel, nModel);

		//"f_step1"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "f_step1" + ".txt";

		saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_f_step1_m, nTest, nModel);

		//"f_step2"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "f_step2" + ".txt";

		saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_f_step2_m, nTest, _DATA._one_t);

		//"Vf_step1"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "Vf_step1" + ".txt";

		saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_Vf_step1_m, nTest, nTest);

		//"Vf_step2"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "Vf_step2" + ".txt";

		saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_Vf_step2_m, nTest, nTest);

		//"inliers"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "inliers" + ".txt";

		saveMatrix(outputFolder + _fileName, inliers, _DATA.Sp_dim_t, _DATA._one_t);


		if (_eval.ocl_Eval_counter.host[0] == _DATA._zero_i)
		{

			size_t nOP = _VELODYNEOPTIONS.N_size;

			//K_OP_X_m

			_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "K_OP_X" + ".txt";

			saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_K_OP_X_m, nOP, nModel);

			//f_OP_step1_m

			_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "f_OP_step1" + ".txt";

			saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_f_OP_step1_m, nOP, nModel);

			//f_OP_step2_m

			_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "f_OP_step2" + ".txt";

			saveMatrix(outputFolder + _fileName, _eval.ocl_matrix_f_OP_step2_m, nOP, _DATA._one_t);

			//"ocl_OP"

			_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "OP" + ".txt";

			saveMatrix(outputFolder + _fileName, ocl_OP.ocl_OP.host, nOP,_DATA._one_t);


		}

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";
	}

}
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, float & number)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo) && ((_LOGINFOOPTIONS.SanityCheckSector == _DATA._zero_i) || (_LOGINFOOPTIONS.SanityCheckSector == _DATA.iS)))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string outputFolder;
		std::string _fileName;

		if (_DATA.iS == _DATA._zero_i)
		{
			outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";
			_fileName = _DATAOPTIONS.fileName + "-" + codeStep + ".txt";
		}
		else
		{
			std::string folder_SanityCheck = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_SanityCheck) || !std::experimental::filesystem::exists(folder_SanityCheck)) {
				std::experimental::filesystem::create_directory(folder_SanityCheck);
			}

			std::string folder_Sectors = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "Sectors\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_Sectors) || !std::experimental::filesystem::exists(folder_Sectors)) {
				std::experimental::filesystem::create_directory(folder_Sectors);
			}

			outputFolder = folder_Sectors + "\\Sector-" + std::to_string(_DATA.iS) + "\\";
			_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + ".txt";
		}

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		std::ofstream out(outputFolder + _fileName, std::ofstream::out | std::ofstream::trunc);

		out << number << std::scientific;
		
		out.close();

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";

	}
}
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, struct Sectors_struct & ocl_PGi, struct Sectors_struct & ocl_Sp, struct PGi_struct(&PGi)[361], int inliers[])
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo) && ((_LOGINFOOPTIONS.SanityCheckSector == _DATA._zero_i) || (_LOGINFOOPTIONS.SanityCheckSector == _DATA.iS)))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string folder_SanityCheck = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";

		//create directory
		if (!std::experimental::filesystem::is_directory(folder_SanityCheck) || !std::experimental::filesystem::exists(folder_SanityCheck)) {
			std::experimental::filesystem::create_directory(folder_SanityCheck);
		}

		std::string folder_Sectors = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "Sectors\\";

		//create directory
		if (!std::experimental::filesystem::is_directory(folder_Sectors) || !std::experimental::filesystem::exists(folder_Sectors)) {
			std::experimental::filesystem::create_directory(folder_Sectors);
		}

		std::string outputFolder = folder_Sectors + "\\Sector-" + std::to_string(_DATA.iS) + "\\";
		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";
		std::string _fileName;

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		//"ocl_Sp"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "Sp" + ".txt";

		saveSector(outputFolder + _fileName, ocl_Sp, _DATA.Sp_dim_t);

		//"ocl_PGi"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "PGi" + ".txt";

		saveSector(outputFolder + _fileName, ocl_PGi, PGi[_DATA.iS].PGi_dim_t);

		//"inliers"

		_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + "-" + "inliers" + ".txt";

		saveMatrix(outputFolder + _fileName, inliers, PGi[_DATA.iS].PGi_dim_t, _DATA._one_t);

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";
	
	}
}
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, struct Sectors_struct & ocl_Struct, size_t dim)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo) && ((_LOGINFOOPTIONS.SanityCheckSector == _DATA._zero_i) || (_LOGINFOOPTIONS.SanityCheckSector == _DATA.iS)))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string outputFolder;
		std::string _fileName;

		if (_DATA.iS == _DATA._zero_i)
		{
			outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";
			_fileName = _DATAOPTIONS.fileName + "-" + codeStep + ".txt";
		}
		else
		{
			std::string folder_SanityCheck = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_SanityCheck) || !std::experimental::filesystem::exists(folder_SanityCheck)) {
				std::experimental::filesystem::create_directory(folder_SanityCheck);
			}

			std::string folder_Sectors = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "Sectors\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_Sectors) || !std::experimental::filesystem::exists(folder_Sectors)) {
				std::experimental::filesystem::create_directory(folder_Sectors);
			}

			outputFolder = folder_Sectors + "\\Sector-" + std::to_string(_DATA.iS) + "\\";
			_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + ".txt";
		}

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		saveSector(outputFolder + _fileName, ocl_Struct, dim);

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";

	}
}
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, float host[], size_t dim_X, size_t dim_Y)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo) && ((_LOGINFOOPTIONS.SanityCheckSector == _DATA._zero_i) || (_LOGINFOOPTIONS.SanityCheckSector == _DATA.iS)))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string outputFolder;
		std::string _fileName;

		if (_DATA.iS == _DATA._zero_i)
		{
			outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";
			_fileName = _DATAOPTIONS.fileName + "-" + codeStep + ".txt";
		}
		else
		{
			std::string folder_SanityCheck = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_SanityCheck) || !std::experimental::filesystem::exists(folder_SanityCheck)) {
				std::experimental::filesystem::create_directory(folder_SanityCheck);
			}

			std::string folder_Sectors = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "Sectors\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_Sectors) || !std::experimental::filesystem::exists(folder_Sectors)) {
				std::experimental::filesystem::create_directory(folder_Sectors);
			}

			outputFolder = folder_Sectors + "\\Sector-" + std::to_string(_DATA.iS) + "\\";
			_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + ".txt";
		}

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		saveMatrix(outputFolder + _fileName, host, dim_X, dim_Y);

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";

	}
}
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, int ground[], float Z[], int Bin[], int idx[], size_t dim_X, size_t dim_Y, int iSector)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo) && ((_LOGINFOOPTIONS.SanityCheckSector == _DATA._zero_i) || (_LOGINFOOPTIONS.SanityCheckSector == _DATA.iS)))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string outputFolder;
		std::string _fileName;

		if (_DATA.iS == _DATA._zero_i)
		{
			outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";
			_fileName = _DATAOPTIONS.fileName + "-" + codeStep + ".txt";
		}
		else
		{

			std::string folder_SanityCheck = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_SanityCheck) || !std::experimental::filesystem::exists(folder_SanityCheck)) {
				std::experimental::filesystem::create_directory(folder_SanityCheck);
			}

			std::string folder_Sectors = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "Sectors\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_Sectors) || !std::experimental::filesystem::exists(folder_Sectors)) {
				std::experimental::filesystem::create_directory(folder_Sectors);
			}

			outputFolder = folder_Sectors + "\\Sector-" + std::to_string(iSector) + "\\";
			_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(iSector)) + "-" + codeStep + ".txt";
		}

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		std::ofstream out(outputFolder + _fileName, std::ofstream::out | std::ofstream::trunc);

		out << "Z" << std::scientific;
		out << "," << std::scientific;
		out << "Bin" << std::scientific;
		out << "," << std::scientific;
		out << "Not-Ground" << std::scientific;
		out << "," << std::scientific;
		out << "idx" << std::scientific;
		out << std::endl;

		for (size_t ix = 0; ix < dim_X; ix++)
		{
			int g = ground[ix];
			float z = Z[ix];
			int b = Bin[ix];
			int IDX = idx[ix];
			
			out << z << std::scientific;
			out << "," << std::scientific;
			out << b << std::scientific;
			out << "," << std::scientific;
			out << g << std::scientific;
			out << "," << std::scientific;
			out << IDX << std::scientific;
			
			out << std::endl;

		}

		out.close();

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";

	}
}
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, int & _S, float & _X, float & _Y, float & _Z, float & R, float & G, float & B, int & idx)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo) && ((_LOGINFOOPTIONS.SanityCheckSector == _DATA._zero_i) || (_LOGINFOOPTIONS.SanityCheckSector == _DATA.iS)))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string outputFolder;
		std::string _fileName;

		if (_DATA.iS == _DATA._zero_i)
		{
			outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";
			_fileName = _DATAOPTIONS.fileName + "-" + codeStep + ".txt";
		}
		else
		{

			std::string folder_SanityCheck = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_SanityCheck) || !std::experimental::filesystem::exists(folder_SanityCheck)) {
				std::experimental::filesystem::create_directory(folder_SanityCheck);
			}

			std::string folder_Sectors = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "Sectors\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_Sectors) || !std::experimental::filesystem::exists(folder_Sectors)) {
				std::experimental::filesystem::create_directory(folder_Sectors);
			}

			outputFolder = folder_Sectors + "\\Sector-" + std::to_string(_LOGINFOOPTIONS.SanityCheckSector) + "\\";
			_fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(_DATA.iS)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + ".txt";
		}

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		std::ofstream out(outputFolder + _fileName, std::ofstream::out | std::ofstream::app);

		out << _S << std::scientific;
		out << "," << std::scientific;
		out << _X << std::scientific;
		out << "," << std::scientific;
		out << _Y << std::scientific;
		out << "," << std::scientific;
		out << _Z << std::scientific;
		out << "," << std::scientific;
		out << R << std::scientific;
		out << "," << std::scientific;
		out << G << std::scientific;
		out << "," << std::scientific;
		out << B << std::scientific;
		out << "," << std::scientific;
		out << idx << std::scientific;
		out << std::endl;

		out.close();

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";

	}
}
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, float X[], float Y[], float Z[], int Bins[], int original_idx[], size_t dim, int Sector)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo) && ((_LOGINFOOPTIONS.SanityCheckSector == _DATA._zero_i) || (_LOGINFOOPTIONS.SanityCheckSector == _DATA.iS)))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string folder_SanityCheck = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";

		//create directory
		if (!std::experimental::filesystem::is_directory(folder_SanityCheck) || !std::experimental::filesystem::exists(folder_SanityCheck)) {
			std::experimental::filesystem::create_directory(folder_SanityCheck);
		}

		std::string folder_Sectors = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "Sectors\\";

		//create directory
		if (!std::experimental::filesystem::is_directory(folder_Sectors) || !std::experimental::filesystem::exists(folder_Sectors)) {
			std::experimental::filesystem::create_directory(folder_Sectors);
		}

		std::string outputFolder = folder_Sectors + "\\Sector-" + std::to_string(Sector) + "\\";
		std::string _fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(Sector)) + "-" + codeStep + ".txt";

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		std::ofstream out(outputFolder + _fileName, std::ofstream::out | std::ofstream::trunc);

		out << "X" << std::scientific;
		out << "," << std::scientific;
		out << "Y" << std::scientific;
		out << "," << std::scientific;
		out << "Z" << std::scientific;
		out << "," << std::scientific;
		out << "Bin" << std::scientific;
		out << "," << std::scientific;
		out << "idx" << std::scientific;
		out << std::endl;

		for (size_t ix = 0; ix < dim; ix++)
		{
			float x = X[ix];
			float y = Y[ix];
			float z = Z[ix];
			int b = Bins[ix];
			int idx = original_idx[ix];

			
			out << x << std::scientific;
			out << "," << std::scientific;
			out << y << std::scientific;
			out << "," << std::scientific;
			out << z << std::scientific;
			out << "," << std::scientific;
			out << b << std::scientific;
			out << "," << std::scientific;
			out << idx << std::scientific;
			out << std::endl;
			

		}

		out.close();

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";

	}
}

void SanityCheckSector(bool shouldDo, std::string codeStep, float Vf_step2, float TModel, bool Vf_test, float Test_Z, float f_step2, float sigma_n, float TData, bool f_test, int Sector, int iZ)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo) && ((_LOGINFOOPTIONS.SanityCheckSector == _DATA._zero_i) || (_LOGINFOOPTIONS.SanityCheckSector == _DATA.iS)))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string folder_SanityCheck = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";

		//create directory
		if (!std::experimental::filesystem::is_directory(folder_SanityCheck) || !std::experimental::filesystem::exists(folder_SanityCheck)) {
			std::experimental::filesystem::create_directory(folder_SanityCheck);
		}

		std::string folder_Sectors = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "Sectors\\";

		//create directory
		if (!std::experimental::filesystem::is_directory(folder_Sectors) || !std::experimental::filesystem::exists(folder_Sectors)) {
			std::experimental::filesystem::create_directory(folder_Sectors);
		}

		std::string outputFolder = folder_Sectors + "\\Sector-" + std::to_string(Sector) + "\\";
		std::string _fileName = _DATAOPTIONS.fileName + "-PGi-Sector-" + (std::to_string(Sector)) + "-Gauss-" + (std::to_string(_DATA.iG)) + "-" + codeStep + ".txt";

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		if (_LOGINFOOPTIONS.verbose)
		{
			std::cout << "<" + division_str + ">\n";
			std::cout << "<" + codeStep + ">\n";
			std::cout << "<" << msg_start << ">";
		}

		std::ofstream out(outputFolder + _fileName, std::ofstream::out | std::ofstream::app);

		if ( (_DATA.iG == _DATA._one_i) && (iZ == _DATA._zero_i) )
		{
			out << "Vf_step2" << std::scientific;
			out << "," << std::scientific;
			out << "TModel" << std::scientific;
			out << "," << std::scientific;
			out << "Vf_test" << std::scientific;
			out << "," << std::scientific;
			out << "Test_Z" << std::scientific;
			out << "," << std::scientific;
			out << "f_step2" << std::scientific;
			out << "," << std::scientific;
			out << "sigma_n" << std::scientific;
			out << "," << std::scientific;
			out << "TData" << std::scientific;
			out << "," << std::scientific;
			out << "f_test" << std::scientific;
			out << std::endl;
		}
		
		out << Vf_step2 << std::scientific;
		out << "," << std::scientific;
		out << TModel << std::scientific;
		out << "," << std::scientific;
		out << Vf_test << std::scientific;
		out << "," << std::scientific;
		out << Test_Z << std::scientific;
		out << "," << std::scientific;
		out << f_step2 << std::scientific;
		out << "," << std::scientific;
		out << sigma_n << std::scientific;
		out << "," << std::scientific;
		out << TData << std::scientific;
		out << "," << std::scientific;
		out << f_test << std::scientific;
		out << std::endl;

		out.close();

		if (_LOGINFOOPTIONS.verbose)
		{
			std::cout << "<" << msg_end << ">\n";
			std::cout << "<" + division_str + ">\n";
		}

	}
}

void SaveSanityCheckBin(bool shouldDo, std::string codeStep, float X[], float Y[], float Z[], int Sectors[], int original_idx[], size_t dim, int Bin)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo) && ((_LOGINFOOPTIONS.SanityCheckBin == _DATA._zero_i) || (_LOGINFOOPTIONS.SanityCheckBin == Bin)))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string folder_SanityCheck = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";

		//create directory
		if (!std::experimental::filesystem::is_directory(folder_SanityCheck) || !std::experimental::filesystem::exists(folder_SanityCheck)) {
			std::experimental::filesystem::create_directory(folder_SanityCheck);
		}

		std::string folder_Bins = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "\\Bins\\";

		//create directory
		if (!std::experimental::filesystem::is_directory(folder_Bins) || !std::experimental::filesystem::exists(folder_Bins)) {
			std::experimental::filesystem::create_directory(folder_Bins);
		}

		std::string outputFolder = folder_Bins + "\\Bin-" + std::to_string(Bin) + "\\";
		std::string _fileName = _DATAOPTIONS.fileName + "-PGi-Bin-" + (std::to_string(Bin)) + "-" + codeStep + ".txt";

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		std::ofstream out(outputFolder + _fileName, std::ofstream::out | std::ofstream::trunc);

		out << "X" << std::scientific;
		out << "," << std::scientific;
		out << "Y" << std::scientific;
		out << "," << std::scientific;
		out << "Z" << std::scientific;
		out << "," << std::scientific;
		out << "Sector" << std::scientific;
		out << "," << std::scientific;
		out << "idx" << std::scientific;
		out << std::endl;

		for (size_t ix = 0; ix < dim; ix++)
		{
			float x = X[ix];
			float y = Y[ix];
			float z = Z[ix];
			int s = Sectors[ix];
			int idx = original_idx[ix];


			out << x << std::scientific;
			out << "," << std::scientific;
			out << y << std::scientific;
			out << "," << std::scientific;
			out << z << std::scientific;
			out << "," << std::scientific;
			out << s << std::scientific;
			out << "," << std::scientific;
			out << idx << std::scientific;
			out << std::endl;


		}

		out.close();

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";

	}
}
void SaveSanityCheckBin(bool shouldDo, std::string codeStep, int ground[], float Z[], int Sector[], int idx[], size_t dim_X, size_t dim_Y, int iB)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo) && ((_LOGINFOOPTIONS.SanityCheckBin == _DATA._zero_i) || (_LOGINFOOPTIONS.SanityCheckBin == iB)))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string outputFolder;
		std::string _fileName;

		if (_DATA.iS == _DATA._zero_i)
		{
			outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";
			_fileName = _DATAOPTIONS.fileName + "-" + codeStep + ".txt";
		}
		else
		{

			std::string folder_SanityCheck = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_SanityCheck) || !std::experimental::filesystem::exists(folder_SanityCheck)) {
				std::experimental::filesystem::create_directory(folder_SanityCheck);
			}

			std::string folder_Bins = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "\\Bins\\";

			//create directory
			if (!std::experimental::filesystem::is_directory(folder_Bins) || !std::experimental::filesystem::exists(folder_Bins)) {
				std::experimental::filesystem::create_directory(folder_Bins);
			}

			outputFolder = folder_Bins + "\\Bin-" + std::to_string(iB) + "\\";
			_fileName = _DATAOPTIONS.fileName + "-PGi-Bin-" + (std::to_string(iB)) + "-" + codeStep + ".txt";
		}

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		std::ofstream out(outputFolder + _fileName, std::ofstream::out | std::ofstream::trunc);

		out << "Z" << std::scientific;
		out << "," << std::scientific;
		out << "Sector" << std::scientific;
		out << "," << std::scientific;
		out << "Not-Ground" << std::scientific;
		out << "," << std::scientific;
		out << "idx" << std::scientific;
		out << std::endl;

		for (size_t ix = 0; ix < dim_X; ix++)
		{
			int g = ground[ix];
			float z = Z[ix];
			int s = Sector[ix];
			int IDX = idx[ix];

			out << z << std::scientific;
			out << "," << std::scientific;
			out << s << std::scientific;
			out << "," << std::scientific;
			out << g << std::scientific;
			out << "," << std::scientific;
			out << IDX << std::scientific;

			out << std::endl;

		}

		out.close();

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";

	}
}

void SaveSanityCheck(bool shouldDo, std::string codeStep, float host[], size_t dim_X, size_t dim_Y)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string outputFolder;
		std::string _fileName;

		outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";
		_fileName = _DATAOPTIONS.fileName + "-" + codeStep + ".txt";

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		saveMatrix(outputFolder + _fileName, host, dim_X, dim_Y);

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";

	}
}

void SaveSanityCheck(bool shouldDo, std::string codeStep, float data)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string outputFolder;
		std::string _fileName;

		outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";
		_fileName = _DATAOPTIONS.fileName + "-" + codeStep + ".txt";

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		std::ofstream out(outputFolder + _fileName, std::ofstream::out | std::ofstream::trunc);

		out << data;

		out.close();

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";

	}
}

void SaveSanityCheckDims(bool shouldDo, std::string codeStep, std::string event)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string outputFolder;
		std::string _fileName;

		outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";
		_fileName = _DATAOPTIONS.fileName + "-" + "SanityCheckDims" + ".txt";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::ofstream out(outputFolder + _fileName, std::ofstream::out | std::ofstream::app);

		if (codeStep == _LOGINFOOPTIONS.codeSteps.RunModelLoop)
		{
			if (event == "start")

			{
				out << "iFile,iS,iG,iB,PG_dim,PGi_dim,Sp_dim,Snew_dim,Test_dim,Test_dim_tmp\n";
			}

		}

		out << codeStep << ":" << event << ",";

		out << _DATA.iFile; 

		out << ",";

		out << _DATA.iS;

		out << ",";

		out << _DATA.iG;

		out << ",";

		out << _DATA.iB;

		out << ",";

		out << _DATA.PG_dim_i;

		out << ",";

		out << _DATA.PGi_dim_i;

		out << ",";

		out << _DATA.Sp_dim_i;

		out << ",";

		out << _DATA.Snew_dim_i;

		out << ",";

		out << _DATA.Test_dim_i;

		out << ",";

		out << _DATA.Test_dim_tmp_i;

		out << "\n";

		out.close();

	}
}

void SaveSanityCheck(bool shouldDo, std::string codeStep, Eigen::MatrixXf matrix, size_t dim_X, size_t dim_Y)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SAVE MATH SANITY CHECK ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	if ((_LOGINFOOPTIONS.doSanityCheck) && (shouldDo))
	{

		cl_int err = 0;

		std::cout.precision(5);

		std::string outputFolder;
		std::string _fileName;

		outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";
		_fileName = _DATAOPTIONS.fileName + "-" + codeStep + ".txt";

		std::string division_str = "--------------------------------------------------";
		std::string msg_start = "saving Sanity Check...";
		std::string msg_end = "Sanity Check Saved !!";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		std::cout << "<" + division_str + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" << msg_start << ">";

		saveMatrix(outputFolder + _fileName, matrix, dim_X, dim_Y);

		std::cout << "<" << msg_end << ">\n";
		std::cout << "<" + division_str + ">\n";

	}
}

template <typename T>
void saveMatrix(std::string fullfileName, DeviceAndHostMemoryX<T> & matrix, size_t dim_X, size_t dim_Y)
{
	std::ofstream out(fullfileName, std::ofstream::out | std::ofstream::trunc);

	for (size_t ix = 0; ix < dim_X; ix++)
	{
		for (size_t iy = 0; iy < dim_Y; iy++)
		{
			float x = matrix.host[ix + iy * dim_X];

			out << x << std::scientific;
			if (iy < (dim_Y - 1)) { out << ","; };

		}

		out << std::endl;

	}

	out.close();
}

void saveMatrix(std::string fullfileName, float host[], size_t dim_X, size_t dim_Y)
{
	std::ofstream out(fullfileName, std::ofstream::out | std::ofstream::trunc);

	for (size_t ix = 0; ix < dim_X; ix++)
	{
		for (size_t iy = 0; iy < dim_Y; iy++)
		{
			float x = host[ix + iy * dim_X];

			out << x << std::scientific;
			if (iy < (dim_X - 1)) { out << ","; };

		}

		out << std::endl;

	}

	out.close();
}

void saveMatrix(std::string fullfileName, Eigen::MatrixXf matrix, size_t dim_X, size_t dim_Y)
{
	std::ofstream out(fullfileName, std::ofstream::out | std::ofstream::trunc);

	for (size_t ix = 0; ix < dim_X; ix++)
	{
		for (size_t iy = 0; iy < dim_Y; iy++)
		{
			float x = matrix(ix,iy);

			out << x << std::scientific;
			if (iy < (dim_Y - 1)) { out << ","; };

		}

		out << std::endl;

	}

	out.close();
}

void saveMatrix(std::string fullfileName, int host[], size_t dim_X, size_t dim_Y)
{
	std::ofstream out(fullfileName, std::ofstream::out | std::ofstream::trunc);

	for (size_t ix = 0; ix < dim_X; ix++)
	{
		for (size_t iy = 0; iy < dim_Y; iy++)
		{
			int x = host[ix + iy * dim_X];

			out << x << std::scientific;
			if (iy < (dim_X - 1)) { out << ","; };

		}

		out << std::endl;

	}

	out.close();
}
void saveSector(std::string fullfileName, Sectors_struct & sector, size_t dim_X)
{
	std::ofstream out(fullfileName, std::ofstream::out | std::ofstream::trunc);

	out << "Z" << std::scientific;
	out << "," << std::scientific;
	out << "OP" << std::scientific;
	out << "," << std::scientific;
	out << "Bins" << std::scientific;
	out << "," << std::scientific;
	out << "idx" << std::scientific;
	out << std::endl;

	for (size_t ix = 0; ix < dim_X; ix++)
	{
		
		float Z = sector.ocl_Z.host[ix];
		float OP = sector.ocl_OP.host[ix];
		int Bins = sector.ocl_Bins.host[ix];
		int idx = sector.ocl_original_idx.host[ix];

		out << Z << std::scientific;
		out << "," << std::scientific;
		out << OP << std::scientific;
		out << "," << std::scientific;
		out << Bins << std::scientific;
		out << "," << std::scientific;
		out << idx << std::scientific;

		out << std::endl;

	}

	out.close();
}


void SaveParameters(bool shouldDo)
{
	if (shouldDo)
	{
		std::string fullfileName = _DATAOPTIONS.folderOutput + "\\" + "parameters.txt";

		std::ofstream out(fullfileName, std::ofstream::out | std::ofstream::trunc);

		out << "DATA \n";
		out << "--------------------------------------------------------------------------------------\n";
		out << "min_Z (minimum value of Z for threshold): " << _DATA.min_Z << "\n";

		out << "\n";
		out << "\n";

		out << "VELODYNEOPTIONS \n";
		out << "--------------------------------------------------------------------------------------\n";
		out << "M (total number of sectors): " << _VELODYNEOPTIONS.M << "\n";
		out << "N (total number of bins): " << _VELODYNEOPTIONS.N << "\n";
		out << "R (maximum value of radius around 3D LIDAR Laser): " << _VELODYNEOPTIONS.R << "\n";
		out << "PI (pi number): " << _VELODYNEOPTIONS.PI << "\n";
		out << "Rstep (Rstep = R/N): " << _VELODYNEOPTIONS.Rstep << "\n";

		out << "\n";
		out << "\n";

		out << "MODELOPTIONS \n";
		out << "--------------------------------------------------------------------------------------\n";
		out << "length_scale: " << _MODELOPTIONS.length_scale << "\n";
		out << "sigma_f: " << _MODELOPTIONS.sigma_f << "\n";
		out << "sigma_n: " << _MODELOPTIONS.sigma_n << "\n";
		out << "Z_threshold: " << _MODELOPTIONS.Z_threshold << "\n";
		out << "Hg_threshold: " << _MODELOPTIONS.Hg_threshold << "\n";
		out << "TData: " << _MODELOPTIONS.TData << "\n";
		out << "TModel: " << _MODELOPTIONS.TModel << "\n";
		out << "Tg: " << _MODELOPTIONS.Tg << "\n";

		out << "\n";
		out << "\n";

		out << "DATAOPTIONS \n";
		out << "--------------------------------------------------------------------------------------\n";
		if (_DATAOPTIONS.oneFile)
		{
			out << "fileRunning;: " << _DATAOPTIONS.fileRunning << "\n";
		}
		else
		{
			out << "folderInput: " << _DATAOPTIONS.folderInput << "\n";
		}
		out << "folderOutput: " << _DATAOPTIONS.folderOutput << "\n";
		out << "folderPictures: " << _DATAOPTIONS.folderPictures << "\n";
		out << "folderCamCalibration: " << _DATAOPTIONS.folderCamCalibration << "\n";

		out << "\n";
		out << "\n";

		out << "GPUOPTIONS \n";
		out << "--------------------------------------------------------------------------------------\n";
		out << "platform: " << _GPUOPTIONS.platform << "\n";
		out << "type: " << _GPUOPTIONS.type << "\n";
		out << "device: " << _GPUOPTIONS.device << "\n";
		out << "arithmetic: " << _GPUOPTIONS.arithmetic << "\n";

		out << "\n";
		out << "\n";

		out << "GPU (device info) \n";
		out << "--------------------------------------------------------------------------------------\n";
		
		std::string DEVICE_NAME_str(&_GPUOPTIONS.param_DEVICE_NAME);

		out << "param_DEVICE_NAME: " << DEVICE_NAME_str << "\n";

		out << "param_MAX_WORK_GROUP_SIZE: " << _GPUOPTIONS.param_MAX_WORK_GROUP_SIZE << "\n";
		out << "param_MAX_WORK_ITEM_DIMENSIONS: " << _GPUOPTIONS.param_MAX_WORK_ITEM_DIMENSIONS << "\n";
		out << "param_MAX_WORK_ITEM_SIZES[3]: " << _GPUOPTIONS.param_MAX_WORK_ITEM_SIZES[0] << ":" << _GPUOPTIONS.param_MAX_WORK_ITEM_SIZES[1] << ":" << _GPUOPTIONS.param_MAX_WORK_ITEM_SIZES[2] << "\n";
		out << "param_MIN_WORK_GROUP_SIZE: " << _GPUOPTIONS.param_MIN_WORK_GROUP_SIZE << "\n";
		out << "param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE: " << _GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE << "\n";
		out << "param_SMALLER_MIN_WORK_GROUP_SIZE: " << _GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE << "\n";

		
		out.close();

	}

}

//////////////////////////////////////////////////////////////////////////////////////////////
// OPENCL OBJECTS ////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void startOpenCL()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// CREATE OPENCL CONTEXT + QUEUE + DEVICE ////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	cl_int err = 0;

	_OPENCLOBJECT.queue_properties_multiple_kernels[0] = { CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE };
	_OPENCLOBJECT.queue_properties_multiple_kernels[1] = { 0 };
	_OPENCLOBJECT.queue_properties_multiple_kernels[2] = { 0 };

	_OPENCLOBJECT.queue_properties = 0;
	_OPENCLOBJECT.additional_context_props = 0;

	_OPENCLOBJECT.platform = selectPlatform(_GPUOPTIONS.platform.c_str());
	_OPENCLOBJECT.device = selectDevice(_GPUOPTIONS.device.c_str(), _GPUOPTIONS.type.c_str());

	_OPENCLOBJECT.context = NULL;
	_OPENCLOBJECT.context = clCreateContext(_OPENCLOBJECT.additional_context_props.get(), 1, &_OPENCLOBJECT.device, 0, 0, &err);
	SAMPLE_CHECK_ERRORS(err);

	//char[] param_DEVICE_OPENCL_C_VERSION;
	err = clGetDeviceInfo(_OPENCLOBJECT.device, CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &_GPUOPTIONS.param_DEVICE_OPENCL_C_VERSION_LENGTH);

	_GPUOPTIONS.param_DEVICE_OPENCL_C_VERSION = make_unique<char[]>(_GPUOPTIONS.param_DEVICE_OPENCL_C_VERSION_LENGTH);

	std::string ocl20 = "OpenCL C 2.0";

	_GPUOPTIONS.param_CL_C_VERSION_2_0 = make_unique<char[]>(ocl20.length() + 1);

	std::strcpy(_GPUOPTIONS.param_CL_C_VERSION_2_0.get(), ocl20.c_str());

	ocl20.clear();

	err = clGetDeviceInfo(_OPENCLOBJECT.device, CL_DEVICE_OPENCL_C_VERSION, _GPUOPTIONS.param_DEVICE_OPENCL_C_VERSION_LENGTH, _GPUOPTIONS.param_DEVICE_OPENCL_C_VERSION.get(), NULL);

	int ItIsOpenCL20 = 0;
	ItIsOpenCL20 = std::strcmp(_GPUOPTIONS.param_CL_C_VERSION_2_0.get(), _GPUOPTIONS.param_DEVICE_OPENCL_C_VERSION.get());

	if (ItIsOpenCL20 == 0)
	{
		_OPENCLOBJECT.queue_multiple_kernels = clCreateCommandQueueWithProperties(_OPENCLOBJECT.context, _OPENCLOBJECT.device, _OPENCLOBJECT.queue_properties_multiple_kernels, &err);
		SAMPLE_CHECK_ERRORS(err);

		_OPENCLOBJECT.queue = clCreateCommandQueue(_OPENCLOBJECT.context, _OPENCLOBJECT.device, _OPENCLOBJECT.queue_properties, &err);
		SAMPLE_CHECK_ERRORS(err);
	}
	else
	{
		_OPENCLOBJECT.queue_multiple_kernels = clCreateCommandQueue(_OPENCLOBJECT.context, _OPENCLOBJECT.device, _OPENCLOBJECT.queue_properties, &err);
		SAMPLE_CHECK_ERRORS(err);

		_OPENCLOBJECT.queue = clCreateCommandQueue(_OPENCLOBJECT.context, _OPENCLOBJECT.device, _OPENCLOBJECT.queue_properties, &err);
		SAMPLE_CHECK_ERRORS(err);
	}



}

void DeviceInfo()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// QUERY OPENCL DEVICE INFORMATION ///////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	cl_int err = 0; 

	//size_t param_DEVICE_NAME_LENGTH;

	err = clGetDeviceInfo(_OPENCLOBJECT.device, CL_DEVICE_NAME, 0, 0, &_GPUOPTIONS.param_DEVICE_NAME_LENGTH);

	//char param_DEVICE_NAME;

	err = clGetDeviceInfo(_OPENCLOBJECT.device, CL_DEVICE_NAME, _GPUOPTIONS.param_DEVICE_NAME_LENGTH + _DATA._one_t, &_GPUOPTIONS.param_DEVICE_NAME, NULL);

	//size_t param_MAX_WORK_GROUP_SIZE;

	err = clGetDeviceInfo(_OPENCLOBJECT.device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &_GPUOPTIONS.param_MAX_WORK_GROUP_SIZE,NULL);
			
	//cl_uint param_MAX_WORK_ITEM_DIMENSIONS;

	err = clGetDeviceInfo(_OPENCLOBJECT.device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &_GPUOPTIONS.param_MAX_WORK_ITEM_DIMENSIONS,NULL);

	//size_t param_MAX_WORK_ITEM_SIZES;

	err = clGetDeviceInfo(_OPENCLOBJECT.device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*3, &_GPUOPTIONS.param_MAX_WORK_ITEM_SIZES,NULL);

	//size_t param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE;

	clGetKernelWorkGroupInfo(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, _OPENCLOBJECT.device, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), &_GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, NULL);

		
};

void startGPU()
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	//CREATE GPU/CPU OPENCL CONTEXT/////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

	startOpenCL();

	defineProgramsAndKernels();

	DeviceInfo();

	////////////////////////////////////////////////////////////////////////////////////////////////

}

void stopGPU()
{

	//////////////////////////////////////////////////////////////////////////////////
	//RELEASE OPENCL OBJECTS /////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////

	releaseProgramsAndKernels();

	releaseOpenCL();

	//////////////////////////////////////////////////////////////////////////////////

}

void defineProgramsAndKernels()
{
		cl_int err = 0;

		_OPENCLOBJECT.all_program = OpenCLProgram(_GPUOPTIONS.all_kernelsFILE, "", "");

		// -----------------------------------------------------------------------
		// POLARGRID FUNCTION
		// -----------------------------------------------------------------------

		_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins = NULL;
		_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.PolarGrid_getOPSectorsAndBins.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);
	
		// -----------------------------------------------------------------------
		// SEED FUNCTION
		// -----------------------------------------------------------------------

		_OPENCLOBJECT.my_kernels.Seed_copySp = NULL;
		_OPENCLOBJECT.my_kernels.Seed_copySp = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.Seed_copySp.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

		_OPENCLOBJECT.my_kernels.Seed_SpUSnew_Step1 = NULL;
		_OPENCLOBJECT.my_kernels.Seed_SpUSnew_Step1 = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.Seed_SpUSnew_Step1.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

		_OPENCLOBJECT.my_kernels.Seed_SpUSnew_Step2 = NULL;
		_OPENCLOBJECT.my_kernels.Seed_SpUSnew_Step2 = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.Seed_SpUSnew_Step2.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

		// -----------------------------------------------------------------------
		// TEST FUNCTION
		// -----------------------------------------------------------------------

		_OPENCLOBJECT.my_kernels.Test_getTest_Step1 = NULL;
		_OPENCLOBJECT.my_kernels.Test_getTest_Step1 = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.Test_getTest_Step1.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

		_OPENCLOBJECT.my_kernels.Test_getTest_Step2 = NULL;
		_OPENCLOBJECT.my_kernels.Test_getTest_Step2 = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.Test_getTest_Step2.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

	
		// -----------------------------------------------------------------------
		// REGRESSION FUNCTION
		// -----------------------------------------------------------------------

		_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel = NULL;
		_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.Gauss_SquareExponentialKernel.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

		_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise = NULL;
		_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.Gauss_SquareExponentialKernelNoise.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);
		
		// -----------------------------------------------------------------------
		// EVAL FUNCTIONS
		// -----------------------------------------------------------------------

		_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply = NULL;
		_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.GEMM_matrixMultiply.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

		_OPENCLOBJECT.my_kernels.Eval_matrixVector = NULL;
		_OPENCLOBJECT.my_kernels.Eval_matrixVector = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.Eval_matrixVector.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

		_OPENCLOBJECT.my_kernels.Eval_matrixSubtraction = NULL;
		_OPENCLOBJECT.my_kernels.Eval_matrixSubtraction = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.Eval_matrixSubtraction.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

		_OPENCLOBJECT.my_kernels.Eval_matrixCopy = NULL;
		_OPENCLOBJECT.my_kernels.Eval_matrixCopy = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.Eval_matrixCopy.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

		// -----------------------------------------------------------------------
		// MATRIX INVERSION FUNCTIONS
		// -----------------------------------------------------------------------

		_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM = NULL;
		_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.matrixInverse_sGEMM.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

		_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM = NULL;
		_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.matrixInverse_sTRSM.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);

		_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK = NULL;
		_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK = clCreateKernel(_OPENCLOBJECT.all_program, _GPUOPTIONS.matrixInverse_sSYRK.c_str(), &err);
		SAMPLE_CHECK_ERRORS(err);
		

}

void releaseProgramsAndKernels()
{
	cl_int err = 0;

	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins = NULL;

	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.Seed_copySp);
	SAMPLE_CHECK_ERRORS(err);
	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.Seed_SpUSnew_Step1);
	SAMPLE_CHECK_ERRORS(err);
	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.Seed_SpUSnew_Step2);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.my_kernels.Seed_copySp = NULL;
	_OPENCLOBJECT.my_kernels.Seed_SpUSnew_Step1 = NULL;
	_OPENCLOBJECT.my_kernels.Seed_SpUSnew_Step2 = NULL;

	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.Test_getTest_Step1);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.my_kernels.Test_getTest_Step1 = NULL;

	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.Test_getTest_Step2);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.my_kernels.Test_getTest_Step2 = NULL;

	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel = NULL;

	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise = NULL;

	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply = NULL;

	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.Eval_matrixVector);
	SAMPLE_CHECK_ERRORS(err);
	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.Eval_matrixSubtraction);
	SAMPLE_CHECK_ERRORS(err);
	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.Eval_matrixCopy);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.my_kernels.Eval_matrixVector = NULL;
	_OPENCLOBJECT.my_kernels.Eval_matrixSubtraction = NULL;
	_OPENCLOBJECT.my_kernels.Eval_matrixCopy = NULL;

	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM = NULL;

	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM = NULL;

	err = clReleaseKernel(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK = NULL;



	err = clReleaseProgram(_OPENCLOBJECT.all_program);
	SAMPLE_CHECK_ERRORS(err);

	_OPENCLOBJECT.all_program = NULL;


}

void releaseOpenCL()
{

	//////////////////////////////////////////////////////////////////////////////////////////////
	// RELEASE OPENCL CONTEXT + QUEUE + DEVICE ///////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	cl_int err = 0;

	_GPUOPTIONS.param_CL_C_VERSION_2_0.reset();
	_GPUOPTIONS.param_DEVICE_OPENCL_C_VERSION.reset();

	_OPENCLOBJECT.queue_properties_multiple_kernels[0] = NULL;
	_OPENCLOBJECT.queue_properties_multiple_kernels[1] = NULL;
	_OPENCLOBJECT.queue_properties_multiple_kernels[2] = NULL;
	_OPENCLOBJECT.queue_properties = NULL;
	_OPENCLOBJECT.additional_context_props.reset();

	err = clReleaseDevice(_OPENCLOBJECT.device);
	SAMPLE_CHECK_ERRORS(err);

	err = clReleaseCommandQueue(_OPENCLOBJECT.queue);
	SAMPLE_CHECK_ERRORS(err);

	err = clReleaseCommandQueue(_OPENCLOBJECT.queue_multiple_kernels);
	SAMPLE_CHECK_ERRORS(err);

	err = clReleaseContext(_OPENCLOBJECT.context);
	SAMPLE_CHECK_ERRORS(err);
	
	_OPENCLOBJECT.alignmentForPtr = NULL;
	_OPENCLOBJECT.platform = NULL;
	_OPENCLOBJECT.device = NULL;
	_OPENCLOBJECT.queue = NULL;
	_OPENCLOBJECT.context = NULL;


}

std::string get_build_options(std::string arithmetic, int TILE_GROUP_M, int TILE_SIZE_M, int TILE_GROUP_N, int TILE_SIZE_N, int TILE_SIZE_K)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// BUILD OPENCL PROGRAM BUILD OPTIONS FOR INTEL GEMM /////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	std::string build_options = "-DT=" + arithmetic +
		" -DTILE_SIZE_M=" + std::to_string(TILE_SIZE_M) +
		" -DTILE_GROUP_M=" + std::to_string(TILE_GROUP_M) +
		" -DTILE_SIZE_N=" + std::to_string(TILE_SIZE_N) +
		" -DTILE_GROUP_N=" + std::to_string(TILE_GROUP_N) +
		" -DTILE_SIZE_K=" + std::to_string(TILE_SIZE_K);

	return build_options;
};

void getGWWIandADDPAD(size_t dim_X, size_t dim_Y, size_t dim_Z)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// DEFINE GLOBAL_SIZE AND LOCAL_SIZE  ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	size_t dim[3] = { dim_X, dim_Y, dim_Z };
	size_t _mul[3] = { 0,0,0 };
	size_t mul;

	size_t dim_size;

	_GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE = (size_t)8;

	for (int dim_idx = 0; dim_idx < 3; dim_idx++)
	{
		dim_size = dim[dim_idx];

		if (static_cast<int>(dim_size) > (static_cast<int>(_GPUOPTIONS.param_MAX_WORK_GROUP_SIZE)*static_cast<int>(_GPUOPTIONS.param_MAX_WORK_GROUP_SIZE)))
		{
			_GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE = _GPUOPTIONS.param_MAX_WORK_GROUP_SIZE;
		}

		if (dim_size == _DATA._one_t)
		{
			_DATA.global_size[dim_idx] = _DATA._one_t;
			_DATA.local_size[dim_idx] = _DATA._one_t;

			_DATA.dim_pad[dim_idx] = 0;
		}
		else if ( static_cast<int>(dim_size) > static_cast<int>(_GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE) )
		{
			int _dim_size = static_cast<int>(dim_size);
			int _wgs = static_cast<int>(_GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE);
			int _mul = static_cast<int>(ceil(_dim_size / _wgs));
			mul = static_cast<size_t>(_mul);

			mul = mul + _DATA._one_t;

			_DATA.global_size[dim_idx] = _GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE * mul;
			_DATA.local_size[dim_idx] = _GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE;

			_DATA.dim_pad[dim_idx] = (mul * _GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE) - dim_size;

			while (_DATA.local_size[dim_idx] < _GPUOPTIONS.param_MAX_WORK_GROUP_SIZE)
			{
				_DATA.local_size[dim_idx] = _DATA.local_size[dim_idx] + _GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE;
			}

		}
		else if (static_cast<int>(dim_size) < static_cast<int>(_GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE))
		{
			int _dim_size = static_cast<int>(dim_size);
			int _wgs = static_cast<int>(_GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE);
			int _mul = static_cast<int>(ceil(_dim_size / _wgs));
			mul = static_cast<size_t>(_mul);

			mul = mul + _DATA._one_t;

			_DATA.global_size[dim_idx] = _GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE * mul;
			_DATA.local_size[dim_idx] = _GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE;

			_DATA.dim_pad[dim_idx] = (mul * _GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE) - dim_size;

		}

	}

	bool condition_Max_Work_Group_Total_Size = ( (static_cast<int>(_DATA.local_size[0]) * static_cast<int>(_DATA.local_size[1]) * static_cast<int>(_DATA.local_size[2])) > static_cast<int>(_GPUOPTIONS.param_MAX_WORK_GROUP_SIZE));
	bool condition_Local_Size_Multiple_One = ( (static_cast<int>(_DATA.local_size[0]) % static_cast<int>(_GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE)) != _DATA._zero_i) && ( (static_cast<int>(_DATA.local_size[0]) != _DATA._one_i) );
	bool condition_Local_Size_Multiple_Two = ( (static_cast<int>(_DATA.local_size[1]) % static_cast<int>(_GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE)) != _DATA._zero_i) && ((static_cast<int>(_DATA.local_size[1]) != _DATA._one_i));
	bool condition_Local_Size_Multiple_Three = (( static_cast<int>(_DATA.local_size[2]) % static_cast<int>(_GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE)) != _DATA._zero_i) && ((static_cast<int>(_DATA.local_size[2]) != _DATA._one_i));
	bool condition_GlobalLocal_Size_Multiple_One = (static_cast<int>(_DATA.global_size[0]) % static_cast<int>(_DATA.local_size[0])) != _DATA._zero_i;
	bool condition_GlobalLocal_Size_Multiple_Two = (static_cast<int>(_DATA.global_size[1]) % static_cast<int>(_DATA.local_size[1])) != _DATA._zero_i;
	bool condition_GlobalLocal_Size_Multiple_Three = (static_cast<int>(_DATA.global_size[2]) % static_cast<int>(_DATA.local_size[2])) != _DATA._zero_i;

	while ((condition_Max_Work_Group_Total_Size) || (condition_Local_Size_Multiple_One) || (condition_Local_Size_Multiple_Two) || (condition_Local_Size_Multiple_Three) || (condition_GlobalLocal_Size_Multiple_One) || (condition_GlobalLocal_Size_Multiple_Two) || (condition_GlobalLocal_Size_Multiple_Three) )
	{
		int max_idx = 0;

		if (_DATA.local_size[0] > _DATA.local_size[1])
		{
			if (_DATA.local_size[0] > _DATA.local_size[2])
			{
				max_idx = 0;
			}
			else
			{
				max_idx = 2;
			}
		}
		else if (_DATA.local_size[1] > _DATA.local_size[2])
		{
			max_idx = 1;
		}
		else
		{
			max_idx = 2;
		}

		_DATA.local_size[max_idx] = _DATA.local_size[max_idx] - _DATA._one_t;

		condition_Max_Work_Group_Total_Size = ((static_cast<int>(_DATA.local_size[0]) * static_cast<int>(_DATA.local_size[1]) * static_cast<int>(_DATA.local_size[2])) > static_cast<int>(_GPUOPTIONS.param_MAX_WORK_GROUP_SIZE));
		condition_Local_Size_Multiple_One = ((static_cast<int>(_DATA.local_size[0]) % static_cast<int>(_GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE)) != _DATA._zero_i) && ((static_cast<int>(_DATA.local_size[0]) != _DATA._one_i));
		condition_Local_Size_Multiple_Two = ((static_cast<int>(_DATA.local_size[1]) % static_cast<int>(_GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE)) != _DATA._zero_i) && ((static_cast<int>(_DATA.local_size[1]) != _DATA._one_i));
		condition_Local_Size_Multiple_Three = ((static_cast<int>(_DATA.local_size[2]) % static_cast<int>(_GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE)) != _DATA._zero_i) && ((static_cast<int>(_DATA.local_size[2]) != _DATA._one_i));
		condition_GlobalLocal_Size_Multiple_One = (static_cast<int>(_DATA.global_size[0]) % static_cast<int>(_DATA.local_size[0])) != _DATA._zero_i;
		condition_GlobalLocal_Size_Multiple_Two = (static_cast<int>(_DATA.global_size[1]) % static_cast<int>(_DATA.local_size[1])) != _DATA._zero_i;
		condition_GlobalLocal_Size_Multiple_Three = (static_cast<int>(_DATA.global_size[2]) % static_cast<int>(_DATA.local_size[2])) != _DATA._zero_i;

	}
}

void getGEMMparams(int dim_X, int dim_Y, int dim_K)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// DEFINE INTEL GEMM PARAMETERS //////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	size_t size = dim_X * dim_Y;

	_GPUOPTIONS.TILE_SIZE_M = dim_X;
	_GPUOPTIONS.TILE_SIZE_N = dim_Y;

	_DATA.global_size[0] = (size_t)(size / _GPUOPTIONS.TILE_SIZE_M);
	_DATA.global_size[1] = (size_t)(size / _GPUOPTIONS.TILE_SIZE_N);

	_GPUOPTIONS.TILE_GROUP_M = static_cast<int>(_DATA.local_size[0]);
	_GPUOPTIONS.TILE_GROUP_N = static_cast<int>(_DATA.local_size[1]);

	_GPUOPTIONS.TILE_SIZE_K = dim_K;

}

void showDeviceInfo()
{
	std::string division = "--------------------------------------------------";
	std::string DEVICE_NAME_str(&_GPUOPTIONS.param_DEVICE_NAME);

	if (_LOGINFOOPTIONS.showDeviceInfo)
	{
		
		std::cout << "<" + division + ">\n";
		std::cout << "<DEVICE NAME:><" + DEVICE_NAME_str + ">\n";
		std::cout << "<" + division + ">\n";
		std::cout << "<MAX_WORK_GROUP_SIZE:><" << _GPUOPTIONS.param_MAX_WORK_GROUP_SIZE << ">\n";
		std::cout << "<" + division + ">\n";
		std::cout << "<MAX_WORK_ITEM_DIMENSIONS:><" << _GPUOPTIONS.param_MAX_WORK_ITEM_DIMENSIONS << ">\n";
		std::cout << "<" + division + ">\n";
		std::cout << "<MAX_MAX_WORK_ITEM_SIZES:><" << _GPUOPTIONS.param_MAX_WORK_ITEM_SIZES[0] << "-" << _GPUOPTIONS.param_MAX_WORK_ITEM_SIZES[1] << "-" << _GPUOPTIONS.param_MAX_WORK_ITEM_SIZES[2] << ">\n";
		std::cout << "<" + division + ">\n";
		std::cout << "<PREFERRED_WORK_GROUP_SIZE_MULTIPLE:><" << _GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE << ">\n";
		std::cout << "<" + division + ">\n";
		std::cout << "<DEVICE_OPENCL_C_VERSION:><" << _GPUOPTIONS.param_DEVICE_OPENCL_C_VERSION << ">\n";
		std::cout << "<" + division + ">\n";

	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// CAM CALIBRATION ///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void loadCalibrationCamToCam()
{
	//open file
	std::unique_ptr<FILE, int(*)(FILE*)> stream(std::fopen(_DATAOPTIONS.CamToCamCalibrationFile.get(), "rb"), &fclose);

	if (stream.get() == NULL)
	{
		//calib = [];
		return;
	}

	//read corner distance
	_VELODYNEOPTIONS._CALIB.cornerdist = readVariable(stream.get(), "corner_dist", 1, 1);
	
	//read all cameras(maximum: 100)
	for (int cam = 1; cam <= 100; cam++)
	{
		std::string cam_str;
		if (cam < 10)
		{
			cam_str = "0" + std::to_string(cam - 1);
		}
		else
		{
			cam_str = std::to_string(cam - 1);
		}

		//read variables
		Eigen::MatrixXf S_ = readVariable(stream.get(), "S_" + cam_str, 1, 2);
		Eigen::MatrixXf K_ = readVariable(stream.get(), "K_" + cam_str, 3, 3);
		Eigen::MatrixXf D_ = readVariable(stream.get(), "D_" + cam_str, 1, 5);
		Eigen::MatrixXf R_ = readVariable(stream.get(), "R_" + cam_str, 3, 3);
		Eigen::MatrixXf T_ = readVariable(stream.get(), "T_" + cam_str, 3, 1);
		Eigen::MatrixXf S_rect_ = readVariable(stream.get(), "S_rect_" + cam_str, 1, 2);
		Eigen::MatrixXf R_rect_ = readVariable(stream.get(), "R_rect_" + cam_str, 3, 3);
		Eigen::MatrixXf P_rect_ = readVariable(stream.get(), "P_rect_" + cam_str, 3, 4);

		//calibration for this cam completely found ?
		if ((S_.size() == _DATA._zero_t) || (K_.size() == _DATA._zero_t) || (D_.size() == _DATA._zero_t) || (R_.size() == _DATA._zero_t) || (T_.size() == _DATA._zero_t))
		{
			break;
		}

		//write calibration
		_VELODYNEOPTIONS._CALIB.S[cam].CAM = S_;
		_VELODYNEOPTIONS._CALIB.K[cam].CAM = K_;
		_VELODYNEOPTIONS._CALIB.D[cam].CAM = D_;
		_VELODYNEOPTIONS._CALIB.R[cam].CAM = R_;
		_VELODYNEOPTIONS._CALIB.T[cam].CAM = T_;

		//if rectification available
		if ((S_rect_.size() != _DATA._zero_t) && (R_rect_.size() != _DATA._zero_t) && (P_rect_.size() != _DATA._zero_t))
		{
			_VELODYNEOPTIONS._CALIB.S_rect[cam].CAM = S_rect_;
			_VELODYNEOPTIONS._CALIB.R_rect[cam].CAM = R_rect_;
			_VELODYNEOPTIONS._CALIB.P_rect[cam].CAM = P_rect_;
		}
	
	}

	//close file
	stream.reset();
}

void loadCalibrationRigid()
{
	//open file
	std::unique_ptr<FILE, int(*)(FILE*)> stream(std::fopen(_DATAOPTIONS.VeloToCamCalibrationFile.get(), "rb"), &fclose);
	
	if (stream.get() == NULL)
	{
		throw "Erro reading Velo-to-Cam Calibration File";
	}

	//read calibration
	Eigen::MatrixXf R = readVariable(stream.get(), "R", 3, 3);
	Eigen::MatrixXf T = readVariable(stream.get(), "T", 3, 1);

	std::vector<float> tmp = {0,0,0,1};

	int dim_X = 4;
	int dim_Y = 3;

	_VELODYNEOPTIONS.Tr_velo_to_cam.resize(dim_X,dim_X);

	for (int i = 0; i < dim_Y; i++)
	{
		for (int j = 0; j < dim_Y; j++)
		{
			_VELODYNEOPTIONS.Tr_velo_to_cam(i, j) = R(i, j);
		}
		
	}

	for (int i = 0; i < dim_Y; i++)
	{
		_VELODYNEOPTIONS.Tr_velo_to_cam(i, 3) = T(i);
	}

	for (int i = 0; i < dim_X; i++)
	{
		_VELODYNEOPTIONS.Tr_velo_to_cam(3, i) = tmp[i];
	}

	//close file
	stream.reset();

}

void projectionMatrixVeloToImg()
{
	//compute projection matrix velodyne->image plane
	
	_VELODYNEOPTIONS.R_cam_to_rect.resize(4,4);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			
			_VELODYNEOPTIONS.R_cam_to_rect(i,j) = _VELODYNEOPTIONS._CALIB.R_rect[1].CAM(i,j);

		}

	}

	for (int i = 0; i < 4; i++)
	{
		_VELODYNEOPTIONS.R_cam_to_rect(i, 3) = _DATA._zero_f;
	}

	for (int i = 0; i < 4; i++)
	{
		_VELODYNEOPTIONS.R_cam_to_rect(3, i) = _DATA._zero_f;
	}

	_VELODYNEOPTIONS.R_cam_to_rect(3, 3) = _DATA._one_f;

	Eigen::MatrixXf tmp(3,4);
	tmp = _VELODYNEOPTIONS._CALIB.P_rect[_VELODYNEOPTIONS.CAM + 1].CAM * _VELODYNEOPTIONS.R_cam_to_rect;	

	_VELODYNEOPTIONS.P_Velo_to_Img.resize(3,4);
	_VELODYNEOPTIONS.P_Velo_to_Img = tmp * _VELODYNEOPTIONS.Tr_velo_to_cam;

}

Eigen::MatrixXf readVariable(FILE* fid, std::string name, size_t M, size_t N)
{
	int err = 0;
	
	Eigen::MatrixXf A(M,N);
	float val;

	std::string name_str = name + ':';
	std::unique_ptr<char[]> name_char(new char[name_str.length() + 1]);
	std::unique_ptr<char[]> str(new char[20]);

	std::strcpy(name_char.get(), name_str.c_str());

	//rewind
	err = std::fseek(fid, 0, SEEK_SET);

	//search for variable identifier
	int success = 1;
	while (success > 0)
	{
		success = fscanf(fid, "%s", str.get());

		if (std::strcmp(str.get(), name_char.get()) == _DATA._zero_i)
		{
			success = 0;
		}
	}

	//return if variable identifier not found
	if (success > 0)
	{
		return A;
	}

	for (int m = 0; m < M; m++)
	{
		for (int n = 0; n < N; n++)
		{
			success = fscanf(fid, "%f", &val);

			if (success > _DATA._zero_i)
			{
				A(m, n) = val;
			}
			else
			{	//A = [];
				return A;
			}
		}
	}

	name_char.reset();
	str.reset();

	return A;

}

//////////////////////////////////////////////////////////////////////////////////////////////
// MODEL PARAMETERS //////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void setParameters()
{
	_OPENCLOBJECT.alignmentForPtr = 4096; // Please refer to Intel Zero Copy Tutorial and OpenCL Performance Guide

	// -----------------------------------------------------------------------
	// _GPUOPTIONS
	// -----------------------------------------------------------------------


	_GPUOPTIONS.PolarGrid_getOPSectorsAndBins = "PolarGrid_getOPSectorsAndBins";

	_GPUOPTIONS.Seed_copySp = "Seed_copySp";
	_GPUOPTIONS.Seed_SpUSnew_Step1 = "Seed_SpUSnew_Step1";
	_GPUOPTIONS.Seed_SpUSnew_Step2 = "Seed_SpUSnew_Step2";
	_GPUOPTIONS.Seed_generateSp = "Seed_generateSp";

	_GPUOPTIONS.Gauss_SquareExponentialKernel = "Gauss_SquareExponentialKernel";
	_GPUOPTIONS.Gauss_SquareExponentialKernelNoise = "Gauss_SquareExponentialKernelNoise";

	_GPUOPTIONS.Test_getTest_Step1 = "Test_getTest_Step1";
	_GPUOPTIONS.Test_getTest_Step2 = "Test_getTest_Step2";

	_GPUOPTIONS.Eval_matrixCopy = "Eval_matrixCopy";
	_GPUOPTIONS.Eval_matrixMultiply = "Eval_matrixMultiply";
	_GPUOPTIONS.Eval_matrixVector = "Eval_matrixVector";
	_GPUOPTIONS.Eval_matrixSubtraction = "Eval_matrixSubtraction";

	_GPUOPTIONS.GEMM_matrixMultiply = "GEMM_matrixMultiply";

	_GPUOPTIONS.matrixInverse_sGEMM = "matrixInverse_sGEMM";
	_GPUOPTIONS.matrixInverse_sTRSM = "matrixInverse_sTRSM";
	_GPUOPTIONS.matrixInverse_sSYRK = "matrixInverse_sSYRK";

	_GPUOPTIONS.all_kernelsFILE = L"allKernels.cl";

	_GPUOPTIONS.TILE_SIZE_K = 0;

	_GPUOPTIONS.Work_Dim = 3;

	// -----------------------------------------------------------------------
	// _DATA
	// -----------------------------------------------------------------------

	_DATA.dim_pad[0] = { 0 };
	_DATA.dim_pad[1] = { 0 };
	_DATA.dim_pad[2] = { 0 };

	_DATA._zero_t = 0;
	_DATA._one_t = 1;
	_DATA._zero_i = 0;
	_DATA._one_i = 1;
	_DATA._zero_f = 0.f;
	_DATA._one_f = 1.f;
	_DATA._m_one_f = -1.f;
	_DATA.inf = std::numeric_limits<float>::infinity();
	_DATA._false = false;
	_DATA._sign_pos = 1.f;
	_DATA._sign_neg = -1.f;
	_DATA._invert_sign = 0.f;

};

void parseParameters(int argc, const char** argv)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// SET DEFAULT OPTIONS ///////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	bool debug_mode = true; //to run the code with choosing command line options

	//VELODYNE OPTIONS///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float PI = (float)3.1415;
	float M = 360;
	int R = 50;
	int N = 160;
	float Rstep = ((float)R / (float)N);
	
	_VELODYNEOPTIONS.M = M;
	_VELODYNEOPTIONS.M_size = static_cast<size_t>(M);
	_VELODYNEOPTIONS.N = N;
	_VELODYNEOPTIONS.N_size = static_cast<size_t>(N);
	_VELODYNEOPTIONS.R = R;
	_VELODYNEOPTIONS.PI = PI;
	_VELODYNEOPTIONS.Rstep = Rstep;

	_VELODYNEOPTIONS.doOnlyFrontView = true;

	//MODEL OPTIONS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float Z_threshold = (float)8.5;
	float Hg_threshold = (float)0.25;
	float length_scale = (float)0.3;
	float sigma_f = (float)1.3298;
	float sigma_n = (float)0.1;
	float TData = (float)80/100;
	float TModel = (float)0.5/100;
	float Tg = (float)100/100;

	_MODELOPTIONS.length_scale = length_scale;
	_MODELOPTIONS.sigma_f = sigma_f;
	_MODELOPTIONS.sigma_n = sigma_n;
	_MODELOPTIONS.Z_threshold = Z_threshold;
	_MODELOPTIONS.Hg_threshold = Hg_threshold;
	_MODELOPTIONS.TData = TData;
	_MODELOPTIONS.TModel = TModel;
	_MODELOPTIONS.Tg = Tg;

	_MODELOPTIONS.setManualMinZ = false;
	_MODELOPTIONS.manualMinZ = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//FILES OPTIONS: OneFile MODE or ManyFiles MODE  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	_DATAOPTIONS.oneFile = true;
	_DATAOPTIONS.maxNumberOfFiles = 10;

	//_DATAOPTIONS.fileRunning = "";
	//_DATAOPTIONS.folderInput = "";
	//_DATAOPTIONS.folderOutput = "";
	//_DATAOPTIONS.folderPictures = "";

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//REPORT OPTIONS/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	_LOGINFOOPTIONS.verbose = false;
	_LOGINFOOPTIONS.showReport = false;
	_LOGINFOOPTIONS.saveReport = true;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//GPU OPTIONS////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	_GPUOPTIONS.type = "gpu";
	_GPUOPTIONS.device = "0";
	_GPUOPTIONS.arithmetic = "float";
	_GPUOPTIONS.platform = "0";

	_LOGINFOOPTIONS.showDeviceInfo = false;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//RESULT OPTIONS/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	_LOGINFOOPTIONS.saveResult = false;
	_LOGINFOOPTIONS.savePicture = true;
	_LOGINFOOPTIONS.saveMovie = true;
	_LOGINFOOPTIONS.fps = 10;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//SANITY CHECK OPTIONS///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	_LOGINFOOPTIONS.doSaveParameters = false;
	_LOGINFOOPTIONS.doSanityCheck = false;
	_LOGINFOOPTIONS.SanityCheckSector = 10;
	_LOGINFOOPTIONS.doSanityCheckSector = false;
	_LOGINFOOPTIONS.SanityCheckBin = 48;
	_LOGINFOOPTIONS.doSanityCheckBin = false;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//debug_mode runs the code without submitting parameters from command line
	if (debug_mode)
	{

		//KITTI RAW DATA - 2011_09_29_drive_0071_sync
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		_LOGINFOOPTIONS.doSaveParameters = true;
		
		_LOGINFOOPTIONS.doSanityCheck = true;
		_LOGINFOOPTIONS.doSanityCheckSector = true;
		_LOGINFOOPTIONS.SanityCheckSector = 0;
		_LOGINFOOPTIONS.doSanityCheckBin = true;
		_LOGINFOOPTIONS.SanityCheckBin = 0;
		_LOGINFOOPTIONS.doSanityCheckDim = false;

		_DATAOPTIONS.oneFile = true;

		_MODELOPTIONS.setManualMinZ = true;
		_MODELOPTIONS.manualMinZ = (float)-1.6;

		_DATAOPTIONS.maxNumberOfFiles = 1059;

		_LOGINFOOPTIONS.savePicture = true;
		_LOGINFOOPTIONS.saveMovie = false;

		_LOGINFOOPTIONS.saveReport = true;

		_VELODYNEOPTIONS.doOnlyFrontView = true;

		_DATAOPTIONS.fileRunning = "C:\\Users\\Dornas\\Hard Quale .com\\Kod-Demand - Documents\\_CLIENTS\\SEOULROBOTICS\\_DATA\\Velodyne\\Training\\000020.bin";
		//_DATAOPTIONS.folderInput = "C:\\Users\\Dornas\\Hard Quale .com\\Kod-Demand - Documents\\_CLIENTS\\SEOULROBOTICS\\_DATA\\Velodyne\\Training";
		//_DATAOPTIONS.folderInput = "C:\\Users\\Dornas\\Hard Quale .com\\Kod-Demand - Documents\\_CLIENTS\\SEOULROBOTICS\\_DATA\\2011_09_29_drive_0071_sync\\2011_09_29\\2011_09_29_drive_0071_sync\\velodyne_points\\data";
		//_DATAOPTIONS.folderOutput = "C:\\Users\\Dornas\\Hard Quale .com\\Kod-Demand - Documents\\_CLIENTS\\SEOULROBOTICS\\output\\2011_09_29_drive_0071_sync";
		_DATAOPTIONS.folderOutput = "C:\\Users\\Dornas\\Hard Quale .com\\Kod-Demand - Documents\\_CLIENTS\\SEOULROBOTICS\\output";
		_DATAOPTIONS.folderPictures = "C:\\Users\\Dornas\\Hard Quale .com\\Kod-Demand - Documents\\_CLIENTS\\SEOULROBOTICS\\_PICTURE\\Velodyne\\Training";
		//_DATAOPTIONS.folderPictures = "C:\\Users\\Dornas\\Hard Quale.com\\Kod - Demand - Documents\\_CLIENTS\\SEOULROBOTICS\\_DATA\\2011_09_29_drive_0071_sync\\2011_09_29\\2011_09_29_drive_0071_sync\\image_00\\data";

		_DATAOPTIONS.folderCamCalibration = "C:\\Users\\Dornas\\Hard Quale .com\\Kod-Demand - Documents\\_CLIENTS\\SEOULROBOTICS\\_DATA\\Velodyne\\calib";

		std::string camTocam = _DATAOPTIONS.folderCamCalibration + "\\" + "calib_cam_to_cam.txt";
		std::string veloTocam = _DATAOPTIONS.folderCamCalibration + "\\" + "calib_velo_to_cam.txt";

		_DATAOPTIONS.CamToCamCalibrationFile = make_unique<char[]>(camTocam.length() + 1);
		_DATAOPTIONS.VeloToCamCalibrationFile = make_unique<char[]>(veloTocam.length() + 1);

		std::strcpy(_DATAOPTIONS.CamToCamCalibrationFile.get(), camTocam.c_str());
		std::strcpy(_DATAOPTIONS.VeloToCamCalibrationFile.get(), veloTocam.c_str());

		camTocam.clear();
		veloTocam.clear();

		_LOGINFOOPTIONS.parsed_OK = true;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
	else
	{
		//////////////////////////////////////////////////////////////////////////////////////////////
		// PARSE PARAMETERS FROM COMMAND LINE OPTIONS ////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////

		_LOGINFOOPTIONS.parsed_OK = true;

		std::string arg_mode_name = argv[1];
		std::string arg_mode_data = argv[2];

		std::transform(arg_mode_name.begin(), arg_mode_name.end(), arg_mode_name.begin(), ::toupper);
		std::transform(arg_mode_data.begin(), arg_mode_data.end(), arg_mode_data.begin(), ::toupper);

		std::string arg_name;
		std::string arg_data;

		if (argc >= 3)
		{
			if (arg_mode_name == "--MODE")
			{
				if ((arg_mode_data == "ONEFILE") && (argc >= 5))
				{

					_LOGINFOOPTIONS.saveMovie = false;
					_LOGINFOOPTIONS.savePicture = true;

					bool FilePath = false;
					bool OutPutFolder = false;
					bool CameraPictureFolder = false;
					bool CameraCalibrationFolder = false;

					for (int i = 3; i < (argc - 1); i++)
					{
						std::string arg_name = argv[i];
						std::string arg_data = argv[i + 1];

						std::transform(arg_name.begin(), arg_name.end(), arg_name.begin(), ::toupper);
						std::transform(arg_data.begin(), arg_data.end(), arg_data.begin(), ::toupper);

						if (arg_name == "--FILEPATH")
						{
							_DATAOPTIONS.oneFile = true;
							_DATAOPTIONS.fileRunning = arg_data;

							FilePath = true;
						}
						else if (arg_name == "--OUTPUTFOLDER")
						{
							_DATAOPTIONS.folderOutput = arg_data;

							OutPutFolder = true;
						}
						else if (arg_name == "--CAMERAPICTUREFOLDER")
						{
							_DATAOPTIONS.folderPictures = arg_data;

							CameraPictureFolder = true;
						}
						else if (arg_name == "--CAMERACALIBRATIONFILESFOLDER")
						{
							_DATAOPTIONS.folderCamCalibration = arg_data;


							std::string camTocam = _DATAOPTIONS.folderCamCalibration + "\\" + "calib_cam_to_cam.txt";
							std::string veloTocam = _DATAOPTIONS.folderCamCalibration + "\\" + "calib_velo_to_cam.txt";

							_DATAOPTIONS.CamToCamCalibrationFile = make_unique<char[]>(camTocam.length() + 1);
							_DATAOPTIONS.VeloToCamCalibrationFile = make_unique<char[]>(veloTocam.length() + 1);

							std::strcpy(_DATAOPTIONS.CamToCamCalibrationFile.get(), camTocam.c_str());
							std::strcpy(_DATAOPTIONS.VeloToCamCalibrationFile.get(), veloTocam.c_str());

							camTocam.clear();
							veloTocam.clear();



							CameraCalibrationFolder = true;
						}

						checkCommonOptionsParameters(arg_name, arg_data);

					}

					if ((!FilePath) || (!OutPutFolder) || (!CameraPictureFolder) || (!CameraCalibrationFolder))
					{
						_LOGINFOOPTIONS.parsed_OK = false;
					}
				}
				else if (arg_mode_data == "MANYFILES")
				{

					bool FolderPath = false;
					bool OutPutFolder = false;
					bool CameraPictureFolder = false;
					bool CameraCalibrationFolder = false;
					bool maxNumberOfFiles = false;

					for (int i = 3; i < (argc - 1); i++)
					{
						std::string arg_name = argv[i];
						std::string arg_data = argv[i + 1];

						std::transform(arg_name.begin(), arg_name.end(), arg_name.begin(), ::toupper);
						std::transform(arg_data.begin(), arg_data.end(), arg_data.begin(), ::toupper);

						if (arg_name == "--FOLDERPATH")
						{
							_DATAOPTIONS.oneFile = false;
							_DATAOPTIONS.folderInput = arg_data;

							FolderPath = true;

						}
						else if (arg_name == "--OUTPUTFOLDER")
						{
							_DATAOPTIONS.folderOutput = arg_data;

							OutPutFolder = true;
						}
						else if (arg_name == "--CAMERAPICTUREFOLDER")
						{
							_DATAOPTIONS.folderPictures = arg_data;

							CameraPictureFolder = true;
						}
						else if (arg_name == "--CAMERACALIBRATIONFILESFOLDER")
						{
							_DATAOPTIONS.folderCamCalibration = arg_data;

							std::string camTocam = _DATAOPTIONS.folderCamCalibration + "\\" + "calib_cam_to_cam.txt";
							std::string veloTocam = _DATAOPTIONS.folderCamCalibration + "\\" + "calib_velo_to_cam.txt";

							_DATAOPTIONS.CamToCamCalibrationFile = make_unique<char[]>(camTocam.length() + 1);
							_DATAOPTIONS.VeloToCamCalibrationFile = make_unique<char[]>(veloTocam.length() + 1);

							std::strcpy(_DATAOPTIONS.CamToCamCalibrationFile.get(), camTocam.c_str());
							std::strcpy(_DATAOPTIONS.VeloToCamCalibrationFile.get(), veloTocam.c_str());

							camTocam.clear();
							veloTocam.clear();

							CameraCalibrationFolder = true;
						}
						else if (arg_name == "--MAXNUMBEROFFILES")
						{
							_DATAOPTIONS.maxNumberOfFiles = std::stoi(arg_data);

							maxNumberOfFiles = true;
						}

						checkCommonOptionsParameters(arg_name, arg_data);

					}

					if ((!FolderPath) || (!OutPutFolder) || (!CameraPictureFolder) || (!maxNumberOfFiles) || (!CameraCalibrationFolder))
					{
						_LOGINFOOPTIONS.parsed_OK = false;

					}

				}
				else if (arg_mode_data == "SANITYCHECK")
				{
					
					_LOGINFOOPTIONS.doSanityCheck = true;

					_LOGINFOOPTIONS.saveMovie = false;
					_LOGINFOOPTIONS.saveReport = false;

					bool FilePath = false;
					bool OutPutFolder = false;
					bool CameraPictureFolder = false;
					bool CameraCalibrationFolder = false;
					bool doSanityCheckSector = false;
					bool SanityCheckSector = false;
					bool doSanityCheckBin = false;
					bool SanityCheckBin = false;

					for (int i = 3; i < (argc - 1); i++)
					{
						std::string arg_name = argv[i];
						std::string arg_data = argv[i + 1];

						std::transform(arg_name.begin(), arg_name.end(), arg_name.begin(), ::toupper);
						std::transform(arg_data.begin(), arg_data.end(), arg_data.begin(), ::toupper);

						if (arg_name == "--FILEPATH")
						{
							_DATAOPTIONS.oneFile = true;
							_DATAOPTIONS.fileRunning = arg_data;

							FilePath = true;

						}
						else if (arg_name == "--OUTPUTFOLDER")
						{
							_DATAOPTIONS.folderOutput = arg_data;

							OutPutFolder = true;

						}
						else if (arg_name == "--CAMERAPICTUREFOLDER")
						{
							_DATAOPTIONS.folderPictures = arg_data;

							CameraPictureFolder = true;

						}
						else if (arg_name == "--CAMERACALIBRATIONFILESFOLDER")
						{
							_DATAOPTIONS.folderCamCalibration = arg_data;


							std::string camTocam = _DATAOPTIONS.folderCamCalibration + "\\" + "calib_cam_to_cam.txt";
							std::string veloTocam = _DATAOPTIONS.folderCamCalibration + "\\" + "calib_velo_to_cam.txt";

							_DATAOPTIONS.CamToCamCalibrationFile = make_unique<char[]>(camTocam.length() + 1);
							_DATAOPTIONS.VeloToCamCalibrationFile = make_unique<char[]>(veloTocam.length() + 1);

							std::strcpy(_DATAOPTIONS.CamToCamCalibrationFile.get(), camTocam.c_str());
							std::strcpy(_DATAOPTIONS.VeloToCamCalibrationFile.get(), veloTocam.c_str());

							camTocam.clear();
							veloTocam.clear();


							CameraCalibrationFolder = true;

						}
						else if (arg_name == "--DOSANITYCHECKSECTOR")
						{
							if (arg_data == "TRUE")
							{
								_LOGINFOOPTIONS.doSanityCheckSector = true;
							}
							else
							{
								_LOGINFOOPTIONS.doSanityCheckSector = false;
							}

							doSanityCheckSector = true;

						}
						else if (arg_name == "--SANITYCHECKSECTOR")
						{

							_LOGINFOOPTIONS.SanityCheckSector = std::stoi(arg_data);

							SanityCheckSector = true;

						}
						else if (arg_name == "--DOSANITYCHECKBIN")
						{
							if (arg_data == "TRUE")
							{
								_LOGINFOOPTIONS.doSanityCheckBin = true;
							}
							else
							{
								_LOGINFOOPTIONS.doSanityCheckBin = false;
							}

							doSanityCheckBin = true;

						}
						else if (arg_name == "--SANITYCHECKBIN")
						{

							_LOGINFOOPTIONS.SanityCheckBin = std::stoi(arg_data);

							SanityCheckBin = true;

						}

						checkCommonOptionsParameters(arg_name, arg_data);

					}

					if ((!FilePath) || (!OutPutFolder) || (!CameraPictureFolder) || (!doSanityCheckSector) || (!SanityCheckSector) || (!doSanityCheckBin) || (!SanityCheckBin) || (!CameraCalibrationFolder))
					{
						_LOGINFOOPTIONS.parsed_OK = false;

					}
					
				}
				else if (arg_mode_data == "SHOWDEVICEINFO")
				{

					_LOGINFOOPTIONS.showDeviceInfo = true;

				}
				else
				{
					_LOGINFOOPTIONS.parsed_OK = false;
				}

			}
			else
			{
				_LOGINFOOPTIONS.parsed_OK = false;
			}

		}

		if (!_LOGINFOOPTIONS.parsed_OK)
		{
			std::cout << "GROUND SEGMENTATION MODEL (Command Line Options): \n";
			std::cout << "\n";
			std::cout << "(syntax)\n";
			std::cout << "GroundSegmentationModel.exe --mode [mode] --option1 [option1] --option2 [option2] ...optionN [optionN] \n";
			std::cout << "\n";
			std::cout << "[MODE: OneFile]\n";
			std::cout << "\n";
			std::cout << "(options)\n";
			std::cout << "--FilePath [FilePath]\n";
			std::cout << "--OutPutFolder [OutPutFolder]\n";
			std::cout << "--CameraPictureFolder [CameraPictureFolder]\n";
			std::cout << "--CameraCalibrationFilesFolder [path to calib_cam_to_cam.txt and calib_velo_to_cam.txt folder]\n";
			std::cout << "\n";
			std::cout << "(example)\n";
			std::cout << "GroundSegmentationModel.exe --mode OneFile --FilePath C:\\Data\\000000.bin --OutPutFolder C:\\Output --CameraPictureFolder C:\\CameraPictureFolder --CameraCalibrationFilesFolder C:\\CamCalibFolder\n";
			std::cout << "\n";
			std::cout << "[MODE: ManyFiles]\n";
			std::cout << "\n";
			std::cout << "(options)\n";
			std::cout << "--FolderPath [FolderPath]\n";
			std::cout << "--OutPutFolder [OutPutFolder]\n";
			std::cout << "--CameraPictureFolder [CameraPictureFolder]\n";
			std::cout << "--CameraCalibrationFilesFolder [path to calib_cam_to_cam.txt and calib_velo_to_cam.txt folder]\n";
			std::cout << "--maxNumberOfFiles [max number of files]\n";
			std::cout << "\n";
			std::cout << "(example)\n";
			std::cout << "GroundSegmentationModel.exe --mode ManyFiles --FolderPath C:\\Data --OutPutFolder C:\\Output --CameraPictureFolder C:\\CameraPictureFolder --CameraCalibrationFilesFolder C:\\CamCalibFolder --maxNumberOfFiles 50\n";
			std::cout << "\n";
			std::cout << "[MODE: SanityCheck]\n";
			std::cout << "\n";
			std::cout << "(options)\n";
			std::cout << "--FilePath [FilePath]\n";
			std::cout << "--OutPutFolder [OutPutFolder]\n";
			std::cout << "--CameraPictureFolder [CameraPictureFolder]\n";
			std::cout << "--CameraCalibrationFilesFolder [path to calib_cam_to_cam.txt and calib_velo_to_cam.txt folder]\n";
			std::cout << "--doSanityCheckSector [true | false]\n";
			std::cout << "--SanityCheckSector [# of Sector, if equal to 0 runs all Sectors]\n";
			std::cout << "--doSanityCheckBin [true | false]\n";
			std::cout << "--SanityCheckBin [# of Bin, if equal to 0 runs all Bins]\n";
			std::cout << "\n";
			std::cout << "(example)\n";
			std::cout << "GroundSegmentationModel.exe --mode SanityCheck --FilePath C:\\Data\\000000.bin --OutPutFolder C:\\Output --CameraPictureFolder C:\\CameraPictureFolder  --CameraCalibrationFilesFolder C:\\CamCalibFolder --doSanityCheckSector true --SanityCheckSector 65 --doSanityCheckBin true --SanityCheckBin 85\n";
			std::cout << "\n";
			std::cout << "[MODE: ShowDevicesInfo]\n";
			std::cout << "\n";
			std::cout << "(example)\n";
			std::cout << "GroundSegmentationModel.exe --mode ShowDevicesInfo\n";
			std::cout << "\n";
			std::cout << "[Additional Options]\n";
			std::cout << "\n";
			std::cout << "(Parameters)\n";
			std::cout << "--SaveParameters [true | false, Default: false]\n";
			std::cout << "--SaveDims [true | false, Default: false]\n";
			std::cout << "\n";
			std::cout << "(Report)\n";
			std::cout << "--Verbose [true | false, Default: false]\n";
			std::cout << "--ShowReport [true | false, Default: false]\n";
			std::cout << "--SaveReport [true | false, Default: false]\n";
			std::cout << "\n";
			std::cout << "(GPU)\n";
			std::cout << "--Type [gpu | cpu, Default: gpu]\n";
			std::cout << "--Device [#ID of Device, Default: 0]\n";
			std::cout << "--Arithmetic [float | double, Default: float]\n";
			std::cout << "--Platform [#ID of Platform, Default: 0]\n";
			std::cout << "\n";
			std::cout << "(Results)\n";
			std::cout << "--SaveResult [true | false, Default: false]\n";
			std::cout << "--SavePicture [true | false, Default: true]\n";
			std::cout << "--SaveMovie [true | false, Default: true]\n";
			std::cout << "--FramePerSecond [Frame Per Second, Default: 10]\n";
			std::cout << "\n";
			std::cout << "(Ground Segmentation Model Parameters)\n";
			std::cout << "--doOnlyFrontView [true | false, Default: true]";
			std::cout << "--SetManualMinZ [true | false, Default: false]";
			std::cout << "--ManualMinZ [Default: not set]";
			std::cout << "--Z_threshold [Default: 8.5]\n";
			std::cout << "--Hg_threshold [Default: 0.10]\n";
			std::cout << "--length_scale [Default: 0.3]\n";
			std::cout << "--sigma_f [Default: 1.3298]\n";
			std::cout << "--sigma_n [Default: 0.1]\n";
			std::cout << "--TData [Default: 80]\n";
			std::cout << "--TModel [Default: 0.5]\n";
			std::cout << "--Tg [Default: 30]\n";

		}

	}

};

void checkCommonOptionsParameters(std::string arg_name, std::string arg_data)
{
	if (arg_name == "--TYPE")
					{
					_GPUOPTIONS.type = arg_data;
					}
	else if (arg_name == "--DEVICE")
					{
					_GPUOPTIONS.device = arg_data;
					}
	else if (arg_name == "--ARITHMETIC")
					{
					_GPUOPTIONS.arithmetic = arg_data;
					}
	else if (arg_name == "--PLATFORM")
					{
					_GPUOPTIONS.platform = arg_data;
					}
	else if (arg_name == "--VERBOSE")
					{
					if (arg_data == "TRUE")
					{
						_LOGINFOOPTIONS.verbose = true;
					}
					else
					{
						_LOGINFOOPTIONS.verbose = false;
					}
					}
	else if (arg_name == "--SHOWREPORT")
					{
					if (arg_data == "TRUE")
					{
						_LOGINFOOPTIONS.showReport = true;
					}
					else
					{
						_LOGINFOOPTIONS.showReport = false;
					}
					}
	else if (arg_name == "--SAVEREPORT")
					{
					if (arg_data == "TRUE")
					{
						_LOGINFOOPTIONS.saveReport = true;
					}
					else
					{
						_LOGINFOOPTIONS.saveReport = false;
					}
					}
	else if (arg_name == "--SAVERESULT")
					{
					if (arg_data == "TRUE")
					{
						_LOGINFOOPTIONS.saveResult = true;
					}
					else
					{
						_LOGINFOOPTIONS.saveResult = false;
					}
					}
	else if (arg_name == "--SAVEPICTURE")
					{
					if (arg_data == "TRUE")
					{
						_LOGINFOOPTIONS.savePicture = true;
					}
					else
					{
						_LOGINFOOPTIONS.savePicture = false;
					}
					}
	else if (arg_name == "--SAVEMOVIE")
	{
		if (arg_data == "TRUE")
		{
			_LOGINFOOPTIONS.saveMovie = true;
		}
		else
		{
			_LOGINFOOPTIONS.saveMovie = false;
		}
	}
	else if (arg_name == "--FRAMEPERSECOND")
	{

		_LOGINFOOPTIONS.fps = std::stof(arg_data);

	}
	else if (arg_name == "--HG_THRESHOLD")
					{

					_MODELOPTIONS.Hg_threshold = std::stof(arg_data);

					}
	else if (arg_name == "--Z_THRESHOLD")
					{

					_MODELOPTIONS.Z_threshold = std::stof(arg_data);

					}
	else if (arg_name == "--LENGTH_SCALE")
					{

					_MODELOPTIONS.length_scale = std::stof(arg_data);

					}
	else if (arg_name == "--SIGMA_N")
					{

					_MODELOPTIONS.sigma_n = std::stof(arg_data);

					}
	else if (arg_name == "--SIGMA_F")
					{

					_MODELOPTIONS.sigma_f = std::stof(arg_data);

					}
	else if (arg_name == "--TDATA")
					{

					_MODELOPTIONS.TData = std::stof(arg_data);

					}
	else if (arg_name == "--TMODEL")
					{

					_MODELOPTIONS.TModel = std::stof(arg_data);

					}
	else if (arg_name == "--TG")
					{

					_MODELOPTIONS.Tg = std::stof(arg_data);

					}
	else if (arg_name == "--SAVEPARAMETERS")
					{

		if (arg_data == "TRUE")
		{
			_LOGINFOOPTIONS.doSaveParameters = true;
		}
		else
		{
			_LOGINFOOPTIONS.doSaveParameters = false;
		}

					}
	else if (arg_name == "--SAVEDIMS")
					{

					if (arg_data == "TRUE")
					{
						_LOGINFOOPTIONS.doSanityCheckDim = true;
					}
					else
					{
						_LOGINFOOPTIONS.doSanityCheckDim = false;
					}

					}
	else if (arg_name == "--SETMANUALMINZ")
					{

					if (arg_data == "TRUE")
					{
						_MODELOPTIONS.setManualMinZ = true;
					}
					else
					{
						_MODELOPTIONS.setManualMinZ = false;
					}

					}
	else if (arg_name == "--MANUALMINZ")
					{

					_MODELOPTIONS.manualMinZ = std::stof(arg_data);

					}
	else if (arg_name == "--DOONLYFRONTVIEW")
					{

					if (arg_data == "TRUE")
					{
						_VELODYNEOPTIONS.doOnlyFrontView = true;
					}
					else
					{
						_VELODYNEOPTIONS.doOnlyFrontView = false;
					}

					}
}

float getMean(float host[], size_t dim)
{
	size_t count = 0;
	float sum = 0;
	for (size_t i = 0; i < dim; i++)
	{
		if (!std::isnan(host[i]))
		{
			sum += host[i];
			count++;
		}
		
	}

	float avg = sum / static_cast<float>(count);

	return avg;
}

void removeNAN(DeviceAndHostMemoryX<float> & matrix, size_t dim)
{

	for (size_t i = 0; i < dim; i++)
	{
		if (std::isnan(matrix.host[i]))
		{
			matrix.host[i] = 0;
		}
	}

}

float getMeanAbs(float host[], size_t dim)
{

	int count = 0;
	float sum = 0;
	for (size_t i = 0; i < dim; i++)
	{
		if ((host[i] > _DATA._zero_f) && (host[i] < std::numeric_limits<float>::infinity()) && (!std::isnan(host[i])))
		{
			sum += host[i];
			count++;
		}
	}

	float avg = sum / static_cast<float>(count);

	return avg;
}

float getKurtosis(float host[], float avg, float std, size_t dim)
{
	size_t count = 0;
	float sum = 0;
	for (size_t i = 0; i < dim; i++)
	{
		if (!std::isnan(host[i]))
		{
			sum += ( (host[i] - avg) / std ) * ((host[i] - avg) / std) * ((host[i] - avg) / std) * ((host[i] - avg) / std);
			count++;
		}

	}

	float kut = sum / static_cast<float>(count);

	return kut;
}

float getStd(float host[], float mean, size_t dim)
{
	size_t count = 0;
	float sum = 0;
	for (size_t i = 1; i < dim; i++)
	{
		if (!std::isnan(host[i]))
		{
			sum += ((mean - host[i]) * (mean - host[i]));
			count++;
		}
		
	}

	float std = std::sqrt(sum / (count - _DATA._one_t) );

	return std;
}

float getPeakDensity(float host[], float mean, float std, size_t dim)
{

	DeviceAndHostMemoryX<float> zscore; 
	DeviceAndHostMemoryX<float> mutual_distances;

	zscore.allocData(dim,_DATA._one_t,_DATA._one_t);
	mutual_distances.allocData(dim, _DATA._one_t, _DATA._one_t);

	size_t dim_not_nan = 1;

	for (size_t i = 0; i < dim; i++)
	{
		zscore.host[i] = (host[i] - mean)/std;

	}

	for (size_t i = 0; i < dim; i++)
	{
		for (size_t j = 0; j < dim; j++)
		{
			
			mutual_distances.host[i] = mutual_distances.host[i] + std::abs(zscore.host[i] - zscore.host[j]);
			
		}

	}

	float sum = 0;
	float total = 0;
	for (size_t i = 0; i < dim; i++)
	{
		sum += mutual_distances.host[i];
	}

	zscore.releaseDataHost();
	mutual_distances.releaseDataHost();

	return sum;

}

void quicksort(DeviceAndHostMemoryX<float> & matrix, int lo, int hi)
{
	if (lo < hi)
	{
		int p = partition(matrix, lo, hi);

		quicksort(matrix, lo, p - 1);
		quicksort(matrix, p + 1, hi);
	}
}

int partition(DeviceAndHostMemoryX<float> & matrix, int lo, int hi)
{
	float pivot = matrix.host[hi];

	int i = (lo - 1);

		for (int j = lo; j <= hi - 1; j++)
		{
			if (matrix.host[j] <= pivot)
			{
				i++;  
				std::swap(matrix.host[i],matrix.host[j]);
			}
		}

	std::swap(matrix.host[i + 1], matrix.host[hi]);

	return (i + 1);

}

float getMinBin(struct DeviceAndHostMemoryX<float> & Bin, size_t dim)
{
	float min = std::numeric_limits<float>::infinity();

	for (size_t i = 0; i < dim; i++)
	{
		if (min > Bin.host[i])
		{
			min = Bin.host[i];
		}
	}

	return min;

}

void removeOutlier(struct DeviceAndHostMemoryX<float> & Bin, float mean, float std, size_t dim)
{
	
	for (size_t i = 0; i < dim; i++)
	{
		if (std::abs(Bin.host[i]) > (std::abs(mean) + std) )
		{
			Bin.host[i] = mean;
		}
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////
// RELEASE MEMORY OBJECTS ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void releaseLogInfo()
{
	_LOGINFOOPTIONS.cputime_RunModelLoop.count = NULL;
	_LOGINFOOPTIONS.cputime_SectorsLoop.count = NULL;
	_LOGINFOOPTIONS.cputime_GaussLoop.count = NULL;
	_LOGINFOOPTIONS.cputime_SegmentLoop.count = NULL;

	_LOGINFOOPTIONS.cputime_PolarGridFunction.count = NULL;
	_LOGINFOOPTIONS.cputime_sectorFunction.count = NULL;
	_LOGINFOOPTIONS.cputime_getMinZFunction.count = NULL;
	_LOGINFOOPTIONS.cputime_countSeedFunction.count = NULL;
	_LOGINFOOPTIONS.cputime_extractSeedFunction.count = NULL;
	_LOGINFOOPTIONS.cputime_groupSpUSnewFunction.count = NULL;
	_LOGINFOOPTIONS.cputime_GetTestFunction.count = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.count = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.count = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.count = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.count = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.count = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.count = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART7A.count = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART7B.count = NULL;
	_LOGINFOOPTIONS.cputime_Segment_getHg.count = NULL;
	_LOGINFOOPTIONS.cputime_Segment_doSegment.count = NULL;

	_LOGINFOOPTIONS.cputime_RunModelLoop.start = NULL;
	_LOGINFOOPTIONS.cputime_SectorsLoop.start = NULL;
	_LOGINFOOPTIONS.cputime_GaussLoop.start = NULL;
	_LOGINFOOPTIONS.cputime_SegmentLoop.start = NULL;

	_LOGINFOOPTIONS.cputime_PolarGridFunction.start = NULL;
	_LOGINFOOPTIONS.cputime_sectorFunction.start = NULL;
	_LOGINFOOPTIONS.cputime_getMinZFunction.start = NULL;
	_LOGINFOOPTIONS.cputime_countSeedFunction.start = NULL;
	_LOGINFOOPTIONS.cputime_extractSeedFunction.start = NULL;
	_LOGINFOOPTIONS.cputime_groupSpUSnewFunction.start = NULL;
	_LOGINFOOPTIONS.cputime_GetTestFunction.start = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.start = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.start = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.start = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.start = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.start = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.start = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART7A.start = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART7B.start = NULL;
	_LOGINFOOPTIONS.cputime_Segment_getHg.start = NULL;
	_LOGINFOOPTIONS.cputime_Segment_doSegment.start = NULL;

	_LOGINFOOPTIONS.cputime_RunModelLoop.finish = NULL;
	_LOGINFOOPTIONS.cputime_SectorsLoop.finish = NULL;
	_LOGINFOOPTIONS.cputime_GaussLoop.finish = NULL;
	_LOGINFOOPTIONS.cputime_SegmentLoop.finish = NULL;

	_LOGINFOOPTIONS.cputime_PolarGridFunction.finish = NULL;
	_LOGINFOOPTIONS.cputime_sectorFunction.finish = NULL;
	_LOGINFOOPTIONS.cputime_getMinZFunction.finish = NULL;
	_LOGINFOOPTIONS.cputime_countSeedFunction.finish = NULL;
	_LOGINFOOPTIONS.cputime_extractSeedFunction.finish = NULL;
	_LOGINFOOPTIONS.cputime_groupSpUSnewFunction.finish = NULL;
	_LOGINFOOPTIONS.cputime_GetTestFunction.finish = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.finish = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.finish = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.finish = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.finish = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.finish = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.finish = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART7A.finish = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART7B.finish = NULL;
	_LOGINFOOPTIONS.cputime_Segment_getHg.finish = NULL;
	_LOGINFOOPTIONS.cputime_Segment_doSegment.finish = NULL;

	_LOGINFOOPTIONS.cputime_RunModelLoop.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_SectorsLoop.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_GaussLoop.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_SegmentLoop.elapsed = NULL;

	_LOGINFOOPTIONS.cputime_PolarGridFunction.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_sectorFunction.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_getMinZFunction.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_countSeedFunction.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_extractSeedFunction.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_groupSpUSnewFunction.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_GetTestFunction.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART1_STEPS1234.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART2_STEPS56.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART3_STEP7.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART4_STEP8.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART5_STEP9.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART6_STEP10.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART7A.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_Eval_PART7B.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_Segment_getHg.elapsed = NULL;
	_LOGINFOOPTIONS.cputime_Segment_doSegment.elapsed = NULL;
}

void clearVariables()
{

	//GPUOPTIONS

	_GPUOPTIONS.platform.clear();
	_GPUOPTIONS.type.clear();
	_GPUOPTIONS.device.clear();
	_GPUOPTIONS.arithmetic.clear();

	_GPUOPTIONS.PolarGrid_getOPSectorsAndBins.clear();

	_GPUOPTIONS.Seed_copySp.clear();
	_GPUOPTIONS.Seed_SpUSnew_Step1.clear();
	_GPUOPTIONS.Seed_SpUSnew_Step2.clear();
	_GPUOPTIONS.Seed_generateSp.clear();

	_GPUOPTIONS.Gauss_SquareExponentialKernel.clear();
	_GPUOPTIONS.Gauss_SquareExponentialKernelNoise.clear();

	_GPUOPTIONS.Test_getTest_Step1.clear();
	_GPUOPTIONS.Test_getTest_Step2.clear();

	_GPUOPTIONS.Eval_matrixCopy.clear();
	_GPUOPTIONS.Eval_matrixMultiply.clear();
	_GPUOPTIONS.Eval_matrixVector.clear();
	_GPUOPTIONS.Eval_matrixSubtraction.clear();

	_GPUOPTIONS.GEMM_matrixMultiply.clear();

	_GPUOPTIONS.matrixInverse_sGEMM.clear();
	_GPUOPTIONS.matrixInverse_sTRSM.clear();
	_GPUOPTIONS.matrixInverse_sSYRK.clear();

	_GPUOPTIONS.all_kernelsFILE.clear();

	_GPUOPTIONS.param_DEVICE_NAME_LENGTH = NULL;
	_GPUOPTIONS.param_DEVICE_NAME = NULL;
	_GPUOPTIONS.param_MAX_WORK_GROUP_SIZE = NULL;
	_GPUOPTIONS.param_MAX_WORK_ITEM_DIMENSIONS = NULL;
	_GPUOPTIONS.param_MAX_WORK_ITEM_SIZES[0] = NULL;
	_GPUOPTIONS.param_MAX_WORK_ITEM_SIZES[1] = NULL;
	_GPUOPTIONS.param_MAX_WORK_ITEM_SIZES[2] = NULL;
	_GPUOPTIONS.param_MIN_WORK_GROUP_SIZE = NULL;
	_GPUOPTIONS.param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE = NULL;
	_GPUOPTIONS.param_SMALLER_MIN_WORK_GROUP_SIZE = NULL;
	_GPUOPTIONS.param_DEVICE_OPENCL_C_VERSION_LENGTH = NULL;

	_GPUOPTIONS.TILE_GROUP_M = NULL;
	_GPUOPTIONS.TILE_GROUP_N = NULL;
	_GPUOPTIONS.TILE_SIZE_M = NULL;
	_GPUOPTIONS.TILE_SIZE_N = NULL;
	_GPUOPTIONS.TILE_SIZE_K = NULL;

	_GPUOPTIONS.Work_Dim = NULL;

	//VELODYNEOPTIONS

	_VELODYNEOPTIONS.M = NULL;
	_VELODYNEOPTIONS.M_size = NULL;
	_VELODYNEOPTIONS.N = NULL;
	_VELODYNEOPTIONS.N_size = NULL;
	_VELODYNEOPTIONS.R = NULL;

	_VELODYNEOPTIONS.PI = NULL;

	_VELODYNEOPTIONS.Rstep = NULL;

	_VELODYNEOPTIONS.NCoordinates = NULL;
	_VELODYNEOPTIONS.CAM = NULL;

	_VELODYNEOPTIONS._CALIB.cornerdist.resize(0, 0);
	for (int i = 0; i < 101; i++)
	{
		_VELODYNEOPTIONS._CALIB.D[i].CAM.resize(0, 0);
		_VELODYNEOPTIONS._CALIB.K[i].CAM.resize(0, 0);
		_VELODYNEOPTIONS._CALIB.P_rect[i].CAM.resize(0, 0);
		_VELODYNEOPTIONS._CALIB.R[i].CAM.resize(0, 0);
		_VELODYNEOPTIONS._CALIB.R_rect[i].CAM.resize(0, 0);
		_VELODYNEOPTIONS._CALIB.S[i].CAM.resize(0, 0);
		_VELODYNEOPTIONS._CALIB.S_rect[i].CAM.resize(0, 0);
		_VELODYNEOPTIONS._CALIB.T[i].CAM.resize(0, 0);
	}

	_VELODYNEOPTIONS.XYZT_prj.resize(0,0);

	_VELODYNEOPTIONS.P_Velo_to_Img.resize(0, 0);
	_VELODYNEOPTIONS.Tr_velo_to_cam.resize(0, 0);
	_VELODYNEOPTIONS.R_cam_to_rect.resize(0, 0);

	//MODELOPTIONS

	_MODELOPTIONS.length_scale = NULL;
	_MODELOPTIONS.sigma_f = NULL;
	_MODELOPTIONS.sigma_n = NULL;

	_MODELOPTIONS.Z_threshold = NULL;
	_MODELOPTIONS.TData = NULL;
	_MODELOPTIONS.TModel = NULL;
	_MODELOPTIONS.Tg = NULL;

	//DATA

	_DATA.dim_pad[0] = NULL;
	_DATA.dim_pad[1] = NULL;
	_DATA.dim_pad[2] = NULL;

	_DATA._zero_t = NULL;
	_DATA._one_t = NULL;
	_DATA._zero_i = NULL;
	_DATA._one_i = NULL;
	_DATA._zero_f = NULL;
	_DATA._one_f = NULL;
	_DATA._m_one_f = NULL;
	_DATA.inf = NULL;
	_DATA._false = NULL;
	_DATA._sign_pos = NULL;
	_DATA._sign_neg = NULL;
	_DATA._invert_sign = NULL;

	_DATA.gaussLoopStart = NULL;

	_DATA.global_size[0] = NULL;
	_DATA.local_size[0] = NULL;

	_DATA.global_size[1] = NULL;
	_DATA.local_size[1] = NULL;

	_DATA.global_size[2] = NULL;
	_DATA.local_size[2] = NULL;

	//DATAOPTIONS

	_DATAOPTIONS.CamToCamCalibrationFile.reset();
	_DATAOPTIONS.VeloToCamCalibrationFile.reset();

}

void aligned_free(void *aligned)
{
	if (!aligned)return; // behaves as delete: calling with 0 is NOP
	delete[] * ((char**)aligned - 1);
}

void* aligned_malloc(size_t size, size_t alignment)
{
	// a number of requirements should be met
	assert(alignment > 0);
	assert((alignment & (alignment - 1)) == 0); // test for power of 2

	if (alignment < sizeof(void*))
	{
		alignment = sizeof(void*);
	}

	assert(size >= sizeof(void*));
	assert(size / sizeof(void*) * sizeof(void*) == size);

	// allocate extra memory and convert to size_t to perform calculations
	char* orig = new char[size + alignment + sizeof(void*)];
	// calculate an aligned position in the allocated region
	// assumption: (size_t)orig does not lose lower bits
	char* aligned =
		orig + (
		(((size_t)orig + alignment + sizeof(void*)) & ~(alignment - 1)) -
			(size_t)orig
			);
	// save the original pointer to use it in aligned_free
	*((char**)aligned - 1) = orig;
	return aligned;
}

size_t zeroCopySizeAlignment(size_t requiredSize, cl_device_id device)
{
	// Please refer to Intel Zero Copy Tutorial and OpenCL Performance Guide
	// The following statement rounds requiredSize up to the next 64-byte boundary
	return requiredSize + (~requiredSize + 1) % 64;   // or even shorter: requiredSize + (-requiredSize) % 64
}

cl_uint zeroCopyPtrAlignment(cl_device_id device)
{
	// Please refer to Intel Zero Copy Tutorial and OpenCL Performance Guide
	return 4096;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// OPENCL BASIC FUNCTIONS ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

cl_platform_id selectPlatform(const string& platform_name_or_index)
{
	using namespace std;

	cl_uint num_of_platforms = 0;
	cl_int err = clGetPlatformIDs(0, 0, &num_of_platforms);
	SAMPLE_CHECK_ERRORS(err);

	vector<cl_platform_id> platforms(num_of_platforms);
	err = clGetPlatformIDs(num_of_platforms, &platforms[0], 0);
	SAMPLE_CHECK_ERRORS(err);

	cl_uint selected_platform_index = num_of_platforms;
	bool by_index = false;

	if (is_number(platform_name_or_index))
	{
		by_index = true;
		selected_platform_index = str_to<int>(platform_name_or_index);
	}

	string required_platform_subname = platform_name_or_index;

	for (cl_uint i = 0; i < num_of_platforms; ++i)
	{

		size_t platform_name_length = 0;
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			0,
			0,
			&platform_name_length
		);
		SAMPLE_CHECK_ERRORS(err);

		vector<char> platform_name(platform_name_length);
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			platform_name_length,
			&platform_name[0],
			0
		);
		SAMPLE_CHECK_ERRORS(err);

		if (
			selected_platform_index == i ||
			string(&platform_name[0]).find(required_platform_subname) != string::npos &&
			selected_platform_index == num_of_platforms
			)
		{
			selected_platform_index = i;
		}
	}

	if (by_index && selected_platform_index >= num_of_platforms)
	{
		throw Error(
			"Given index of platform (" + platform_name_or_index + ") "
			"is out of range of available platforms"
		);
	}

	if (!by_index && selected_platform_index >= num_of_platforms)
	{
		throw Error(
			"There is no found platform with name containing \"" +
			required_platform_subname + "\" as a substring\n"
		);
	}

	return platforms[selected_platform_index];
};

static bool device_comp(cl_device_id id1, cl_device_id id2)
{
	cl_int err;
	size_t len1, len2;
	err = clGetDeviceInfo(id1, CL_DEVICE_NAME, 0, NULL, &len1);
	SAMPLE_CHECK_ERRORS(err);
	err = clGetDeviceInfo(id2, CL_DEVICE_NAME, 0, NULL, &len2);
	SAMPLE_CHECK_ERRORS(err);
	vector<char>    name1(len1);
	vector<char>    name2(len2);
	err = clGetDeviceInfo(id1, CL_DEVICE_NAME, len1, &name1[0], NULL);
	SAMPLE_CHECK_ERRORS(err);
	err = clGetDeviceInfo(id2, CL_DEVICE_NAME, len2, &name2[0], NULL);
	SAMPLE_CHECK_ERRORS(err);
	return strcmp(&name1[0], &name2[0])>0;
};

std::vector<cl_device_id> selectDevices(cl_platform_id platform, const string& device_type_name)
{
	using namespace std;

	cl_device_type device_type = parseDeviceType(device_type_name);

	cl_uint num_of_devices = 0;
	cl_int err = clGetDeviceIDs(
		platform,
		device_type,
		0,
		0,
		&num_of_devices
	);

	SAMPLE_CHECK_ERRORS(err);

	vector<cl_device_id> devices(num_of_devices);

	err = clGetDeviceIDs(
		platform,
		device_type,
		num_of_devices,
		&devices[0],
		0
	);
	SAMPLE_CHECK_ERRORS(err);

	for (cl_uint i = 0; i < num_of_devices; ++i)
	{
		size_t device_name_length = 0;
		err = clGetDeviceInfo(
			devices[i],
			CL_DEVICE_NAME,
			0,
			0,
			&device_name_length
		);
		SAMPLE_CHECK_ERRORS(err);

		vector<char> device_name(device_name_length);
		err = clGetDeviceInfo(
			devices[i],
			CL_DEVICE_NAME,
			device_name_length,
			&device_name[0],
			0
		);
		SAMPLE_CHECK_ERRORS(err);

	}

	return devices;
};

cl_device_id selectDevice(const string& device_name_or_index, const string& device_type_name)
{
	using namespace std;

	cl_device_id device;

	if (!_OPENCLOBJECT.platform)
	{
		throw Error("Platform is not selected");
	}

	cl_device_type device_type = parseDeviceType(device_type_name);

	cl_uint num_of_devices = 0;
	cl_int err = clGetDeviceIDs(
		_OPENCLOBJECT.platform,
		device_type,
		0,
		0,
		&num_of_devices
	);

	SAMPLE_CHECK_ERRORS(err);

	vector<cl_device_id> devices(num_of_devices);

	err = clGetDeviceIDs(
		_OPENCLOBJECT.platform,
		device_type,
		num_of_devices,
		&devices[0],
		0
	);
	SAMPLE_CHECK_ERRORS(err);

	if (num_of_devices>1)
	{
		sort(devices.begin(), devices.end(), device_comp);
	}

	cl_uint selected_device_index = num_of_devices;
	bool by_index = false;

	if (is_number(device_name_or_index))
	{
		by_index = true;
		selected_device_index = str_to<int>(device_name_or_index);
	}

	string required_device_subname = device_name_or_index;

	for (cl_uint i = 0; i < num_of_devices; ++i)
	{
		size_t device_name_length = 0;
		err = clGetDeviceInfo(
			devices[i],
			CL_DEVICE_NAME,
			0,
			0,
			&device_name_length
		);
		SAMPLE_CHECK_ERRORS(err);

		vector<char> device_name(device_name_length);
		err = clGetDeviceInfo(
			devices[i],
			CL_DEVICE_NAME,
			device_name_length,
			&device_name[0],
			0
		);
		SAMPLE_CHECK_ERRORS(err);

		if (
			(
				by_index &&
				selected_device_index == i
				) ||
				(
					!by_index &&
					string(&device_name[0]).find(required_device_subname) != string::npos &&
					selected_device_index == num_of_devices
					)
			)
		{
			selected_device_index = i;
		}

	}

	if (by_index && selected_device_index >= num_of_devices)
	{
		throw Error(
			"Given index of device (" + device_name_or_index + ") "
			"is out of range of available devices" +
			(device_type != CL_DEVICE_TYPE_ALL ?
				" (among devices of type " + device_type_name + ")" :
				string("")
				)
		);
	}

	if (!by_index && selected_device_index >= num_of_devices)
	{
		throw Error(
			"There is no found device with name containing \"" +
			required_device_subname + "\" as a substring\n"
		);
	}

	device = devices[selected_device_index];

	return device;
};

cl_device_type parseDeviceType(const string& device_type_name)
{
	cl_device_type  device_type = 0;
	for (size_t pos = 0, next = 0; next != string::npos; pos = next + 1)
	{
		next = device_type_name.find_first_of("+|", pos);
		size_t substr_len = (next != string::npos) ? (next - pos) : (string::npos);
		string name = device_type_name.substr(pos, substr_len);
		if (
			name == "all" ||
			name == "ALL" ||
			name == "CL_DEVICE_TYPE_ALL"
			)
		{
			device_type |= CL_DEVICE_TYPE_ALL;
			continue;
		}

		if (
			name == "default" ||
			name == "DEFAULT" ||
			name == "CL_DEVICE_TYPE_DEFAULT"
			)
		{
			device_type |= CL_DEVICE_TYPE_DEFAULT;
			continue;
		}

		if (
			name == "cpu" ||
			name == "CPU" ||
			name == "CL_DEVICE_TYPE_CPU"
			)
		{
			device_type |= CL_DEVICE_TYPE_CPU;
			continue;
		}

		if (
			name == "gpu" ||
			name == "GPU" ||
			name == "CL_DEVICE_TYPE_GPU"
			)
		{
			device_type |= CL_DEVICE_TYPE_GPU;
			continue;
		}

		if (
			name == "acc" ||
			name == "ACC" ||
			name == "accelerator" ||
			name == "ACCELERATOR" ||
			name == "CL_DEVICE_TYPE_ACCELERATOR"
			)
		{
			device_type |= CL_DEVICE_TYPE_ACCELERATOR;
			continue;
		}

		throw Error(
			"Cannot recognize " + device_type_name + " as a device type"
		);
	}
	return device_type;
};

cl_context createContext(const cl_context_properties* additional_context_props)
{
	using namespace std;

	cl_context context;

	if (!_OPENCLOBJECT.platform)
	{
		throw Error("Platform is not selected");
	}

	if (!_OPENCLOBJECT.device)
	{
		throw Error("Device is not selected");
	}

	size_t number_of_additional_props = 0;
	if (additional_context_props)
	{
		while (additional_context_props[number_of_additional_props++]);
		number_of_additional_props--;
	}

	std::vector<cl_context_properties> context_props(
		2 +
		number_of_additional_props +
		1
	);

	context_props[0] = CL_CONTEXT_PLATFORM;
	context_props[1] = cl_context_properties(_OPENCLOBJECT.platform);

	std::copy(
		additional_context_props,
		additional_context_props + number_of_additional_props,
		context_props.begin() + 2
	);

	context_props.back() = 0;

	cl_int err = 0;
	context = clCreateContext(&context_props[0], 1, &_OPENCLOBJECT.device, 0, 0, &err);
	SAMPLE_CHECK_ERRORS(err);

	return context;
};

cl_command_queue createQueue(cl_command_queue_properties queue_properties)
{
	using namespace std;

	cl_command_queue queue;

	if (!_OPENCLOBJECT.device)
	{
		throw Error("Device is not selected");
	}

	cl_int err = 0;
	queue = clCreateCommandQueue(_OPENCLOBJECT.context, _OPENCLOBJECT.device, queue_properties, &err);
	SAMPLE_CHECK_ERRORS(err);

	return queue;
};

cl_command_queue createQueueWithProperties(cl_command_queue_properties queue_properties[])
{
	using namespace std;

	cl_command_queue queue;

	if (!_OPENCLOBJECT.device)
	{
		throw Error("Device is not selected");
	}

	cl_int err = 0;
	queue = clCreateCommandQueueWithProperties(_OPENCLOBJECT.context, _OPENCLOBJECT.device, queue_properties, &err);
	SAMPLE_CHECK_ERRORS(err);

	return queue;
};

void readFile(const std::wstring& file_name, vector<char>& Data)
{
	using namespace std;

	// Read program from a file

	// First, determine where file exists; look at two places:
	//   - current/default directory; also suitable for full paths
	//   - directory where executable is placed
#ifdef __linux__
	//Store current locale and set default locale
	CTYPELocaleHelper locale_helper;

	ifstream file(
		wstringToString(file_name).c_str(),
		ios_base::ate | ios_base::binary
	);
#else
	ifstream file(
		file_name.c_str(),
		ios_base::ate | ios_base::binary
	);
#endif

	if (!file)
	{
		// There are no file at current/default directory or absolute
		// path. Try to open it relatively from the directory where
		// executable binary is placed.


		cerr
			<< "[ WARNING ] Unable to load OpenCL source code file "
			<< inquotes(wstringToString(file_name)) << " at "
			<< "the default location.\nTrying to open the file "
			<< "from the directory with executable...";

		file.clear();

#ifdef __linux__
		std::string dir = exe_dir();
		file.open(
			(dir + wstringToString(file_name)).c_str(),
			ios_base::ate | ios_base::binary
		);

		if (!file)
		{
			cerr << " FAILED\n";
			throw Error(
				"Cannot open file " + inquotes(dir + wstringToString(file_name))
			);
		}
		else
		{
			cerr << " OK\n";
		}
		cerr << "Full file path is " << inquotes(dir + wstringToString(file_name)) << "\n";
#else
		std::wstring dir = exe_dir_w();
		file.open(
			(dir + file_name).c_str(),
			ios_base::ate | ios_base::binary
		);

		if (!file)
		{
			cerr << " FAILED\n";
			throw Error(
				"Cannot open file " + wstringToString(dir + file_name)
			);
		}
		else
		{
			cerr << " OK\n";
		}
		cerr << "Full file path is " << wstringToString(inquotes_w(dir + file_name)) << "\n";
#endif

	}

	// Second, determine the file length
	std::streamoff file_length = file.tellg();

	if (file_length == -1)
	{
		throw Error(
			"Cannot determine the length of file " +
			wstringToString(inquotes_w(file_name))
		);
	}

	file.seekg(0, ios_base::beg);   // go to the file beginning
	Data.resize(static_cast<size_t>(file_length));
	file.read(&Data[0], file_length);
};

void readProgramFile(const std::wstring& program_file_name, vector<char>& program_text_prepared)
{
	readFile(program_file_name, program_text_prepared);
	program_text_prepared.push_back(0);

};

cl_program createAndBuildProgram(const std::vector<char>& program_text_prepared, cl_context& context, size_t num_of_devices, const cl_device_id* devices, const string& build_options)
{

	const char* raw_text = &program_text_prepared[0];

	cl_int err;

	cl_program program = clCreateProgramWithSource(context, 1, &raw_text, 0, &err);
	SAMPLE_CHECK_ERRORS(err);

	err = clBuildProgram(program, (cl_uint)num_of_devices, devices, build_options.c_str(), 0, 0);

	if (err == CL_BUILD_PROGRAM_FAILURE)
	{
		for (size_t i = 0; i < num_of_devices; ++i)
		{
			size_t log_length = 0;
			err = clGetProgramBuildInfo(
				program,
				devices[i],
				CL_PROGRAM_BUILD_LOG,
				0,
				0,
				&log_length
			);
			SAMPLE_CHECK_ERRORS(err);

			vector<char> log(log_length);

			err = clGetProgramBuildInfo(
				program,
				devices[i],
				CL_PROGRAM_BUILD_LOG,
				log_length,
				&log[0],
				0
			);
			SAMPLE_CHECK_ERRORS(err);

			throw Error(
				"Error happened during the build of OpenCL program.\n"
				"Build log:\n" +
				string(&log[0])
			);
		}
	}

	SAMPLE_CHECK_ERRORS(err);

	raw_text = nullptr;

	return program;
};

cl_program OpenCLProgram(const std::wstring& program_file_name, const string& program_text, const string& build_options)
{
	using namespace std;

	cl_program program;

	if (!program_file_name.empty() && !program_text.empty())
	{
		throw Error(
			"Both program file name and program text are specified. "
			"Should be one of them only."
		);
	}

	if (program_file_name.empty() && program_text.empty())
	{
		throw Error(
			"Neither of program file name or program text are specified. "
			"One of them is required."
		);
	}

	assert(program_file_name.empty() + program_text.empty() == 1);

	vector<char> program_text_prepared;

	if (!program_file_name.empty())
	{
		readProgramFile(program_file_name, program_text_prepared);
	}
	else
	{
		program_text_prepared.resize(program_text.length() + 1);  // +1 for terminating zero
		copy(program_text.begin(), program_text.end(), program_text_prepared.begin());
	}

	program = createAndBuildProgram(program_text_prepared, _OPENCLOBJECT.context, 1, &_OPENCLOBJECT.device, build_options);

	return program;
};



	


	

