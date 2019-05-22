function [Const] = sunem_initialise(OutputDirName,debug)
    %sunem_initialise
    %   Usage:
    %           [Const] = sunem_initialise('dipoles', true)
    %
    %   Input Arguments:
    %       OutputDirName
    %           The output directory name (where the Temp folder, etc. will be stored)
    %       debug
    %           A logical indicating whether the FEKO connect application will run in debug
    %           mode or not
    %
    %   Output Arguments:
    %       Const
    %           Updated global struct (Const) containing the correct file
    %           paths, version information, etc.
    %
    %   Description:
    %       Sets up the environment by calling sunem_setup.m
    %
    %   =======================
    %   Written by Danie Ludick on 2015-04-10
    %   Email: dludick@sun.ac.za

    % Clear the workspace (all except the input arguments)
    % The following command works fine in Octave (but not in MATLAB R2009):
    is_octave = exist('OCTAVE_VERSION', 'builtin') ~= 0;
    if (is_octave)
        % -- Octave
        clear -exclusive OutputDirName debug is_octave
    else
        % -- MATLAB
        clearvars -except OutputDirName debug is_octave
    end%if

    close all
    clc

    % Set global directory names
    Const.ProjectPath = pwd;              % Assume this is called from the examples folder
    Const.OutputDirName = OutputDirName;  % All project related output (e.g. messages)
    Const.debug = debug;

    Const.is_octave = is_octave; % Also save this now to a global constant

    % Switch to the /src directory so that FEKOconnect_setup can be called
    cd ..; cd ..; cd src;

    % Call the global setup script to initialise the rest of the environment
    Const = sunem_setup(Const);