function [arn X] = fast_acf2ar(acf,order,w)
% arn = fast_acf2ar(acf,<order>,<w>)
%
% Convert an autocorrelation function to ARN parameters
% 
%   acf - only one is allowed
%   order - default is length(acf) - 1;
%   w - delay weighting. length(w) should be order or greater
%
% Inverse of fast_ar2acf(phi,nmax)
%
% Note: arn(2:order) will be negative to be consitent with aryule
% and levinson. 
%
% Should give the same result as:
%   phi = levinson(acf,order);
% Though may be off if X is not well conditioned.
%
% See also: fast_ar2acf, levinson, aryule
%
% $Id: fast_acf2ar.m,v 1.2 2009/04/09 21:18:54 greve Exp $

%
% fast_acf2ar.m
%
% Original Author: Doug Greve
% CVS Revision Info:
%    $Author: greve $
%    $Date: 2009/04/09 21:18:54 $
%    $Revision: 1.2 $
%
% Copyright (C) 2002-2007,
% The General Hospital Corporation (Boston, MA). 
% All rights reserved.
%
% Distribution, usage and copying of this software is covered under the
% terms found in the License Agreement file named 'COPYING' found in the
% FreeSurfer source code root directory, and duplicated here:
% https://surfer.nmr.mgh.harvard.edu/fswiki/FreeSurferOpenSourceLicense
%
% General inquiries: freesurfer@nmr.mgh.harvard.edu
% Bug reports: analysis-bugs@nmr.mgh.harvard.edu
%

if(nargin < 1 | nargin > 3)
  fprintf('arn = fast_acf2ar(acf,order,w)\n');
  return;
end

if(~exist('order','var')) order = length(acf) - 1; end
if(~exist('w','var')) w = []; end

%y = -acf(2:order+1); 
y = -acf(2:end);
y = y(:);

%acf = acf(1:order);
acf = acf(1:end-1);
acf = acf(:);

X = toeplitz(acf);
X = X(:,1:order);
if(~isempty(w))
  w = w(2:end);
  w = w(:);
  y = w.*y;
  X = X .* repmat(w,[1 size(X,2)]);
end

arn = inv(X'*X)*X'*y;
arn = [1; arn];

return;







